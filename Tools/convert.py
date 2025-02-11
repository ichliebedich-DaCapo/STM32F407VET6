#!/usr/bin/env python3
import os
import re
import glob
import argparse

# 我应该分成两个样式处理表，一个处理屏幕的，比较简单，另一个处理其他组件的文件。
# 因为处理屏幕需要把名称变成scr，其他组件则是真名
# 我应该在处理样式表时定义一个映射表，把组件名映射为对应的样式名，并且把一些函数整合到一起

# 组件定义代码块
widgets_ns = []
# 界面初始化代码块
screen_ns = []
# 组件块代码
components = []


# --------------------------------预处理------------------------------------

# 寻找setup_scr_*函数
def parse_setup_function(content):
    print("searching...")
    pattern = r'void setup_scr_(\w+)\(lv_ui \*ui\)\s*{([^}]+)}'
    match = re.search(pattern, content, re.DOTALL)
    if not match:
        raise ValueError("Could not find setup_scr_* function")
    return match.group(1), match.group(2).strip()

# ---------------------------------工具函数----------------------------------------
# 检测列表里的两个参数，判断是否是某个值，满足下面规则
# 规则1：如果第二个元素是 value，那么省略它
# 规则2：如果第一个元素是 value，第二个元素不是 value，那么两个元素都不能省略
# 规则3：如果两个元素都是 value，那么都省略，返回空字符串
def check_default_arguments_2(parts,value):
    # 去除空白
    last_two = [p.strip() for p in parts]

    if last_two[1] == value:
        # 第二个元素是 value，省略它
        result = [last_two[0]] if last_two[0] != value else []
    elif last_two[0] == value and last_two[1] != value:
        # 第一个元素是 value，第二个元素不是 value，两个元素都不能省略
        result = last_two
    else:
        # 其他情况
        result = [p for p in last_two if p != value]

    # 如果结果为空，返回空字符串；否则用逗号连接结果
    return ', '.join(result) if result else ''


# --------------------------------分步处理代码块------------------------------------

# 处理样式块,已经包含里样式的正确代码，不用在前面加上组件名
def convert_style_calls(blocks, widget_var):
    converted = []
    for block in blocks:
        # 给每一块代码分成若干行
        lines = block.split('\n')
        for line in lines:
            line = line.strip()
            if not line:
                continue

            # 匹配对应函数 第二个是非贪婪捕获，第三个是贪婪捕获，不过在这里都差不多
            match = re.match(r'lv_obj_set_style_(\w+)\(ui->\w+,\s*(.+?),\s*(.*)\);', line)
            if match:
                # 定义一个函数映射表
                prop = match.group(1)
                value = match.group(2)
                state = match.group(3)

                # 处理状态，如果是默认状态，那么就不添加
                param = "" if state == "LV_PART_MAIN|LV_STATE_DEFAULT" else f", {state}"
                # 每个样式前加上一个Tab，是为了与init对齐
                converted.append(f"\t.{prop}({value}{param})")

    return converted


# 处理初始化代码块（不包含创建）
"""
@param init_lines: 包含了各种初始化代码的行
@note: 把各个初始化行解析并转为链式调用
@return 返回的是处理后的列表
"""


def process_init_function(init_lines):
    # 定义处理规则：每个函数对应的参数提取方式和转换逻辑
    group_functions_handlers = {
        'pos_size': {
            'functions': {
                'lv_obj_set_pos': {'type': 'pos', 'arg_indices': [-2, -1]},
                'lv_obj_set_size': {'type': 'size', 'arg_indices': [-2, -1]}
            },
            # 处理函数：当收集齐pos和size后生成代码
            'handler': lambda pos, size: f".pos_size({', '.join(pos + size)})"
        },
    }
    # 组状态存储
    group_data = {'pos_size': {'pos': None, 'size': None}}

    function_handlers = {
        # 独立函数处理
        'lv_obj_add_flag': {
            # 参数到方法的映射表
            'mapping': {
                'LV_OBJ_FLAG_CHECKABLE': {'method': 'checkable', 'args': None},
                'LV_OBJ_FLAG_HIDDEN': {'method': 'hidden', 'args': None},
                'LV_FLAG_SCROLLABLE': {'method': 'scrollable', 'args': None},
                # 可继续扩展...
            },
            # 多阶段处理器
            'processor': lambda parts, mapping: (
                # 先提取原始参数
                parts[-1].strip(),
                # 再检查是否在映射表中，不在就用默认处理
                mapping.get(parts[-1].strip(), {'method': 'add_flag', 'args': "{}"})
            ),
            'template': lambda method, args: (
                f"\n\t\t\t.{method}({args})" if args
                else f"\n\t\t\t.{method}()"
            )
        },
        'lv_obj_align': {
            'mapping': {
                'LV_ALIGN_CENTER': {'method': 'center', 'args': '{}'},
                'LV_ALIGN_RIGHT': {'method': 'right', 'args': '{}'},
                'LV_FLAG_LEFT': {'method': 'left', 'args': '{}'},
            },
            # 多阶段处理器
            'processor': lambda parts, mapping: (
                # 如果存在映射表，那么就只提取最后两个参数
                parts[-3] + check_default_arguments_2(parts[-2:],'0')
                if parts[-3] in mapping
                else check_default_arguments_2(parts[-2:],'0'),
                # 再检查是否在映射表中，不在就用默认处理
                mapping.get(parts[-3].strip(), {'method': 'align', 'args': "{}"})
            ),
            'template': lambda method, args: (
                f"\n\t\t\t.{method}({args})"
            )
        },
        'lv_obj_set_scrollbar_mode': {
            'processor': lambda parts: (
                parts[-1].strip() if parts[-1].strip() != 'LV_SCROLLBAR_MODE_OFF'
                else None
            ),
            'template': "\n\t\t\t.scrollbar_mode({})"
        },
        # 图像按钮特殊处理
        'lv_imagebutton_set_src': {
            'processor': lambda parts: ', '.join([
                p.strip() for p in parts[-4:-1] if p.strip() != 'NULL'
            ]),
            'template': "\n\t\t\t.src({})"
        }
    }

    # 会被跳过的C函数
    skip_functions = 'lv_label_create'

    # 处理初始化代码的主逻辑
    init_code = []
    for line in init_lines:
        # 跳过创建函数和空行
        if skip_functions in line or not line.strip():
            continue

        handled = False
        # 先处理组函数
        for group_name, group_info in group_functions_handlers.items():
            for func_name, config in group_info['functions'].items():
                if func_name in line:
                    parts = line.strip('();').split(',')
                    # 提取参数并存储到组状态里
                    args = [parts[i].strip() for i in config['arg_indices']]
                    group_data[group_name][config['type']] = args
                    handled = True
                    break
            if handled:
                break

        # 是组函数就不是独立函数
        if handled:
            continue

        # 处理独立函数
        for func_name, config in function_handlers.items():
            if func_name in line:
                # 收集括号内的所有参数
                parts = line.strip('();').split(',')

                # 如果存在映射表，则使用映射表
                if 'mapping' in config:
                    raw_arg, method_info = config['processor'](parts, config['mapping'])
                    if method_info['args'] is None:
                        # 无参函数
                        code = config['template'](method_info['method'], '')
                    else:
                        # 有参函数
                        code = config['template'](
                            method_info['method'],
                            method_info['args'].format(raw_arg)
                        )
                    init_code.append(code)
                else:
                    processed_args = config['processor'](parts)
                    # 调用模板将参数嵌入其中,如果是默认值，那么就跳过这个函数行
                    if processed_args is None:
                        break
                    init_code.append(config['template'].format(processed_args))

                handled = True
                break

        # 确保所有初始化函数都被处理
        # if not handled:
        #     raise ValueError(f"Unhandled init function: {line}")

    # 处理组数据生成代码
    for group_name, data in group_data.items():
        if group_name == 'pos_size':
            pos = data['pos']
            size = data['size']
            if pos and size:
                # 将pos和size合并成字符串并插入首位
                init_code.insert(0, "\n\t\t\t" + group_functions_handlers[group_name]['handler'](pos, size))
    return init_code


# 处理组件代码块
"""
@ component_name 组件名称
@ create_line 创建组件的代码行
@ init_lines 初始化组件属性的代码行
@ style_block 样式代码块
"""


def process_component_block(component_name, create_line, init_lines, style_block):
    # 定义组件名前缀映射表
    prefix_map = {
        "lv_obj_create": "obj_",
        "lv_imagebutton_create": "imgbtn_",
        "lv_label_create": "label_",
        "lv_image_create": "img_",
        "lv_btn_create": "btn_"
    }

    # 获取组件名前缀,如果找不到就默认为obj_
    prefix = ""
    for func, p in prefix_map.items():
        if func in create_line:
            prefix = p
            break

    if prefix == "":
        # 报错，确保能及时发现有未入库的函数，以便及时添加
        raise ValueError(f"Could not find prefix for {create_line}")

    # 合成新组件名
    var_name = prefix + component_name

    # 处理初始化行，获取位置大小等信息，如果遇到create就跳过（因为链式调用里会把这个包含进函数里）
    init_chain = process_init_function(init_lines)

    # 处理样式块，样式块是一段段样式初始化代码
    style_code = convert_style_calls(style_block, var_name)

    return {
        "var_name": var_name,
        "init_chain": init_chain,
        "style_code": style_code,
    }


# --------------------------------处理屏幕和组件代码块------------------------------------
"""
处理主屏幕的代码块
@param blocks: 块代码
@notes: 添加进了components
"""


def process_main_screen_blocks(blocks):
    # 处理主组件，从1开始，因为0是注释前面的，是空行
    if len(blocks) >= 3:
        code_block = blocks[2].strip()
        # 以“//Write  style for"为分隔符(样式注释行被去除)，正常情况还剩下初始化代码和样式代码两块
        parts = re.split(r'//Write style for.*', code_block, maxsplit=1)
        # 舍弃初始化代码块，只保留样式块代码
        style_block = parts[1].strip()
        style_code = []
        # 给每一块代码分成若干行,每行舍弃首尾空白符
        lines = style_block.split('\n')
        is_first_line = True
        for line in lines:
            line = line.strip()
            if not line:
                continue

            # 匹配对应函数 第二个是非贪婪捕获，第三个是贪婪捕获，不过在这里都差不多
            match = re.match(r'lv_obj_set_style_(\w+)\(ui->\w+,\s*(.+?),\s*(.*)\);', line)
            if match:
                prop = match.group(1)
                value = match.group(2)
                state = match.group(3)

                # 处理状态，如果是默认状态，那么就不添加
                param = "" if state == "LV_PART_MAIN|LV_STATE_DEFAULT" else f", {state}"
                # 除却第一行外每个样式前加上一个Tab，是为了与init对齐
                if is_first_line:
                    is_first_line = False
                    style_code.append(f".{prop}({value}{param})")
                else:
                    style_code.append(f"\t.{prop}({value}{param})")

        # 处理组件
        component = {
            "var_name": "scr",
            "init_chain": "",
            "style_code": style_code,
        }
        components.append(component)


"""
处理组件的代码块
@param blocks: 块代码
@notes: 添加进了components
"""


def process_component_blocks(blocks):
    # 遍历注释行和代码块，起始为注释行，偶数为代码块，步长为2
    for i in range(3, len(blocks), 2):
        comment_line = blocks[i].strip()
        code_block = blocks[i + 1].strip()

        # 获取组件名
        component_name = comment_line.split()[-1]
        # 以“//Write  style for"为分隔符(样式注释行被去除)
        parts = re.split(r'//Write style for.*', code_block, maxsplit=1)
        # 组件初始化代码每行分割，去除空行  样式块代码以注释为分割符
        creation_block = parts[0].strip().split('\n')
        style_block = re.split(r'//Write style for.*', parts[1])

        # 处理组件
        component = process_component_block(
            component_name,
            creation_block[0],
            creation_block[1:-1],
            style_block
        )
        components.append(component)


# ----------------------------------输出结果处理--------------------------------------------
def generate_output(project_name):
    for comp in components:
        # 组件定义代码
        if comp['var_name'] != "scr":
            widgets_ns.append(f"Component  {comp['var_name']};")

            # 屏幕初始化代码
            screen_code = []
            if comp['init_chain']:
                screen_code.append(
                    f"{comp['var_name']}" + "".join(comp['init_chain'])
                )
            screen_code.extend(comp['style_code'])

            # 给样式代码添加一个分号
            if screen_code:
                screen_code[-1] += ";\n"
            screen_ns.extend(screen_code)

        else:
            screen_code = []
            screen_code.extend(comp['style_code'])
            if screen_code:
                screen_code[0] = "scr" + screen_code[0]
                screen_code[-1] += ";\n"
            screen_ns.extend(screen_code)

    widgets_block = "\n\t".join(widgets_ns)
    screen_block = "\n\t\t".join(screen_ns)

    return f"""#include "ui.hpp"

namespace gui::widgets::main 
{{
    {widgets_block}
}}

// 使用命名空间
using namespace gui::widgets::main;
namespace gui::init 
{{
    void screen()
    {{
        {screen_block}
    }}
    
    void events()
    {{
    
    }}
}}"""


# -------------------------------------主函数--------------------------------------------
def main():
    # 确定文件路径
    parser = argparse.ArgumentParser()
    parser.add_argument("--path", default="../Projects/driversDevelop/ui/generated")
    args = parser.parse_args()

    # 寻找目标文件
    target_files = glob.glob(os.path.join(args.path, "setup_scr_*.c"))
    if not target_files:
        raise FileNotFoundError("No setup files found")
    if len(target_files) > 1:
        raise ValueError("Multiple setup files found")

    # 打开目标文件并读取内容(默认编码为 utf-8)
    with open(target_files[0], 'r', encoding='utf-8') as f:
        content = f.read()

    # 定位 setup_scr_* 函数,并获取工程名和函数体
    project_name, func_body = parse_setup_function(content)
    print("Processing " + project_name)

    # 把函数体按照注释来分割，每个组件都有一个注释行和一个包含初始化和样式代码块
    blocks = re.split(r'(//Write\s+codes?\s+\w+)', func_body)
    # 处理主屏幕和组件代码块
    process_main_screen_blocks(blocks)
    process_component_blocks(blocks)

    # 输出结果
    output = generate_output(project_name)

    with open("ui.cpp", "w", encoding='utf-8') as f:
        f.write(output)


if __name__ == "__main__":
    # 在执行其他命令之前设置代码页为UTF-8
    os.system('chcp 65001')
    main()
