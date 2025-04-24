import os
import glob
from typing import Dict, List
from dataclasses import dataclass

@dataclass
class FunctionCall:
    name: str
    params: List[str]
    source_file: str  # 新增来源文件信息

@dataclass
class VariableAssignment:
    target: str
    value: str
    var_type: str
    source_file: str

class CodeExtractor:
    def __init__(self):
        self.func_pattern = re.compile(
            r"(?P<func>\w+)$(?P<params>.*?)$\s*;?$"
        )
        self.var_pattern = re.compile(
            r"^(?:(\w+)\s*=\s*)?((?:ui->\w+)\s*=\s*.+)$"
        )

    def extract(self, root_path: str, file_pattern: str) -> Dict[str, dict]:
        """
        增强参数处理的核心方法

        参数:
            root_path: 根目录路径(如r"C:\Users\fairy\Desktop\generated")
            file_pattern: 文件名匹配模式(如"setup_scr_*.c")
        """
        # 安全拼接路径
        search_path = self._safe_path_join(root_path, file_pattern)

        results = {}
        for file_path in glob.glob(search_path, recursive=True):
            file_data = self._process_single_file(file_path)
            if file_data:
                results[file_path] = file_data
        return results

    def _safe_path_join(self, root: str, pattern: str) -> str:
        """处理跨平台路径问题"""
        # 统一转换路径分隔符
        normalized_root = os.path.normpath(root)
        # 防止双通配符问题
        clean_pattern = pattern.lstrip(os.sep)
        return os.path.join(normalized_root, clean_pattern)

    def _process_single_file(self, file_path: str) -> dict:
        """处理单个文件"""
        with open(file_path, 'r', encoding='utf-8') as f:
            content = self._preprocess(f.read())
            return self._analyze(content, file_path)

    def _preprocess(self, content: str) -> List[str]:
        """预处理阶段"""
        # 移除注释并标准化
        content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)
        content = re.sub(r'//.*', '', content)
        return [
            line.strip().rstrip(';')
            for line in content.split('\n')
            if line.strip()
        ]

    def _analyze(self, lines: List[str], filename: str) -> dict:
        """核心分析逻辑"""
        calls = []
        variables = []

        for line in lines:
            # 函数调用解析
            if (func_match := self.func_pattern.search(line)):
                name, params = self._parse_function(func_match)
                calls.append(FunctionCall(name, params, filename))

            # 变量赋值解析
            elif (var_match := self.var_pattern.search(line)):
                var_info = self._parse_variable(var_match)
                variables.append(VariableAssignment(
                    var_info['target'],
                    var_info['value'],
                    var_info['type'],
                    filename
                ))

        return {
            "calls": [c.__dict__ for c in calls],
            "variables": [v.__dict__ for v in variables]
        }

    def _parse_function(self, match: re.Match) -> tuple:
        """解析函数调用细节"""
        name = match.group('func')
        params = self._split_params(match.group('params'))
        return name, params

    def _split_params(self, param_str: str) -> List[str]:
        """安全分割参数"""
        # 处理含嵌套括号的情况
        params = []
        current = []
        stack = 0

        for char in param_str + ',':  # 确保处理最后一个参数
            if char == '(': stack += 1
            elif char == ')': stack -= 1

            if char == ',' and stack == 0:
                params.append(''.join(current).strip())
                current = []
            else:
                current.append(char)

        return params

    def _parse_variable(self, match: re.Match) -> dict:
        """解析变量赋值细节"""
        # 结构体成员的特殊处理
        if 'ui->' in match.group(2):
            target, value = match.group(2).split('=', 1)
            return {
                'target': target.strip(),
                'value': value.strip(),
                'type': 'struct_member'
            }
        # 常规变量声明
        else:
            type_part = match.group(1) or ''
            target, value = match.group(2).split('=', 1)
            return {
                'target': target.strip(),
                'value': value.strip(),
                'type': type_part.strip()
            }

# 使用示例
if __name__ == "__main__":
    extractor = CodeExtractor()
    results = extractor.extract(
        root_path=r"C:\Users\fairy\Desktop\generated",
        file_pattern="setup_scr_*.c"
    )

    # 打印结果
    for file_path, data in results.items():
        print(f"\n=== 文件: {os.path.basename(file_path)} ===")
        print(f"路径: {file_path}")

        print("\n函数调用:")
        for call in data['calls']:
            print(f"  {call['name']}({', '.join(call['params'])})")

        print("\n变量赋值:")
        for var in data['variables']:
            print(f"  {var['target']} ({var['var_type']}) = {var['value']}")