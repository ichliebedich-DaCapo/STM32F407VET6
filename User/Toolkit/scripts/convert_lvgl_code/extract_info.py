import os
import glob
import re
from typing import Dict, List
from dataclasses import dataclass


@dataclass
class FunctionCall:
    name: str
    params: List[str]
    source_file: str  # 文件来源信息,便于后面对多屏幕进行处理（如果我做的话）


@dataclass
class VariableAssignment:
    name: str # 变量名
    value: str  # 值
    var_type: str # 变量类型
    source_file: str


def extract_function_call(text):
    # Step 1: 找到函数名与括号开始位置
    match_func_name = re.match(r'^\s*(\w+)\s*\(', text)
    if not match_func_name:
        return None, []

    function_name = match_func_name.group(1)

    # Step 2: 手动追踪括号嵌套层级，找到匹配的右括号
    depth = 0
    start_idx = len(match_func_name.group(0)) - 1  # 起始于 '(' 的位置
    args_str = ""
    for i in range(start_idx, len(text)):
        char = text[i]
        if char == '(' and depth >= 0:
            depth += 1
        elif char == ')' and depth > 0:
            depth -= 1
        elif char == ')' and depth == 0:
            break
        elif depth > 0:
            args_str += char

    # Step 3: 分割参数，支持引号内逗号
    args = []
    in_quotes = False
    current_arg = ""
    for c in args_str:
        if c == '"':
            in_quotes = not in_quotes
            current_arg += c
        elif c == ',' and not in_quotes:
            args.append(current_arg.strip())
            current_arg = ""
        else:
            current_arg += c
    if current_arg.strip():
        args.append(current_arg.strip())

    return function_name, args


def extract( root_path: str, file_pattern: str) -> Dict[str, dict]:
    r"""
    增强参数处理的核心方法

    参数:
        root_path: 根目录路径(如r"C:\Users\fairy\Desktop\generated")
        file_pattern: 文件名匹配模式(如"setup_scr_*.c")
    """
    # 安全拼接路径
    normalized_root = os.path.normpath(root_path)  # 统一转换路径分隔符
    clean_pattern = file_pattern.lstrip(os.sep)  # 防止双通配符问题
    search_path = os.path.join(normalized_root, clean_pattern)

    all_results = {}
    for single_file_path in glob.glob(search_path, recursive=True):
        file_data = _process_single_file(single_file_path) # 返回的是一个包含变量和函数信息的字典
        if file_data:
            all_results[single_file_path] = file_data
    return all_results

def _process_single_file(file_path: str) -> dict:
    """
    处理单个C代码文件，提取特定函数的代码行
    Args:
        file_path: C代码文件路径
    Returns:
        dict: 分析结果
    """
    with open(file_path, 'r', encoding='utf-8') as f:
        # 预处理阶段
        content = f.read()
        # 查找第一个符合条件的函数：void setup_scr_<identifier>(lv_ui *ui)
        # 正则表达式解释：
        # \b\w+ : 匹配返回类型（如 void, int 等）
        # \s+ : 至少一个空白字符
        # setup_scr\w+ : 匹配函数名（如 setup_scr_screen）
        # $$[^)]*$$ : 匹配参数列表（假设在单行中）
        # \s*{ : 匹配函数体起始的大括号
        # (.*?) : 非贪婪匹配函数体内容（包括换行）
        # } : 匹配闭合的大括号
        pattern = r'void setup_scr_(\w+)\(lv_ui \*ui\)\s*{([^}]+)}'
        match = re.search(pattern, content, re.DOTALL)
        if match:
            function_body = match.group(2).strip()
        else:
            print(f"No Function")
            return {}

        # 清洗代码行
        code_lines = []
        for line in function_body.split('\n'):
            # 去除行内注释
            line = re.sub(r'//.*$', '', line)
            # 去除末尾分号和首尾空格
            clean_line = line.rstrip(';').strip()
            if clean_line:
                code_lines.append(clean_line)

        # 分析阶段
        return _analyze(code_lines, file_path)

def _analyze(lines: List[str], filename: str) -> dict:
    """核心分析逻辑"""
    calls = []
    variables = []

    for line in lines:
        # 处理变量赋值
        if '=' in line:
        # 使用正则表达式进行匹配
            pattern = r'^((\w+\s*\*+)\s+)?(\w+(?:->\w+)*)\s*=\s*(.*?);?$'
            match = re.match(pattern, line)
            if match:
                type_part = match.group(2) if match.group(1) else None  # 类型部分（可选）
                name = match.group(3)
                value = match.group(4)
                variables.append(
                    VariableAssignment(
                        name=name,# 变量名
                        value=value,# 值
                        var_type=type_part,
                        source_file=filename
                    )
                )


        # 处理函数调用
        elif '(' in line and ')' in line:
            func, args = extract_function_call(line)
            calls.append(FunctionCall(name=func, params=args,source_file=filename))


        # 处理纯变量声明
        else:
            pattern = r'(\w+)\*\s+(\w+)'
            match = re.match(pattern, line)
            if match:
                type_name = match.group(1) + '*'  # 拼接类型名和星号
                variable_name = match.group(2)
                variables.append(
                    VariableAssignment(
                        name=variable_name,
                        value="",
                        var_type=type_name,
                        source_file=filename
                    )
                )

    return {
        "calls": [c.__dict__ for c in calls],
        "variables": [v.__dict__ for v in variables]
    }




# 使用示例
if __name__ == "__main__":
    results = extract(
        root_path=r"C:\Users\fairy\Desktop\generated",
        file_pattern="setup_scr_*.c"
    )

    # 打印结果
    for file_path, data in results.items():
        print(f"\r\n=== 文件: {os.path.basename(file_path)} ===")
        print(f"路径: {file_path}")

        print("\r\n函数调用:")
        for call in data['calls']:
            print(f"    {call['name']}    {', '.join(call['params'])}")

        print("\r\n变量赋值:")
        for var in data['variables']:
            print(f"---->  [{var['name']}]  |  [({var['var_type']})]  |  [{var['value']}]")
