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
# 组件代码块
components = []
# 组件的关系, 格式为 [['子组件名', [子组件类型], '父组件名'], ...]
components_relations = []


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
def check_default_arguments_2(parts, value):
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


# 输入组件创建代码块，返回组件前缀和组件类型
def get_widget_type(create_line):
    """
    匹配代码行对应的组件类型
    :param create_line: lv_xxx_create函数所在代码行
    :return:
    """
    widget_map = {
        "lv_obj_create": ["obj", 'Component'],
        "lv_imagebutton_create": ["imgbtn", 'ImageButton'],
        "lv_label_create": ["label", 'Label'],
        "lv_image_create": ["img", 'Image'],
        "lv_btn_create": ["btn", 'Button']
    }
    for func, widget_info in widget_map.items():
        if func in create_line:
            return widget_info

    # 报错，确保能及时发现有未入库的函数，以便及时添加
    raise ValueError(f"Could not find prefix for {create_line}")


# 词法解析,解析代码行的赋值函数语句
# 返回的是变量名，函数名，参数列表（已去除两端空白字符）
def parse_function_line(code_line):
    # 正则表达式模式
    pattern = r'(?:([a-zA-Z_][a-zA-Z0-9_->]*)\s*=\s*)?(\w+)$([^)]*)$'

    # 使用正则表达式进行匹配
    match = re.search(pattern, code_line)

    if match:
        var_name = match.group(1)  # 等号左边的变量名，如果不存在则为 None
        func_name = match.group(2)  # 函数名
        args_str = match.group(3).strip()  # 参数字符串，并去除两端空白字符

        # 如果参数字符串不为空，则按逗号分割参数
        if args_str:
            args = [arg.strip() for arg in args_str.split(',')]
        else:
            args = []  # 如果没有参数，则返回空列表

        return var_name, func_name, args
    else:
        return None, None, None  # 如果没有匹配到，返回 None


def extract_right_variable_name(var_name):
    """
    从变量名字符串中提取箭头右边的部分。
    :param var_name: 包含箭头的变量名字符串
    :return: 箭头右边的变量名部分
    """

    # 查找箭头的位置
    arrow_index = var_name.find('->')

    if arrow_index != -1:
        # 如果找到了箭头，返回箭头右边的部分
        return var_name[arrow_index + 2:]
    else:
        # 如果没有找到箭头，返回原始字符串
        return var_name


def add_component_relations(child_name, child_type, parent_name):
    """
    添加组件关系（支持类型列表）
    :param child_name: 子组件名
    :param child_type: 子组件类型列表（如 ["UI", "Button"]）
    :param parent_name: 父组件名
    :return: True表示添加成功，False表示已存在
    """
    # 自动包装单元素类型为列表（可选）
    if not isinstance(child_type, list):
        child_type = [child_type]

    new_relation = [child_name, child_type, parent_name]
    if new_relation in components_relations:
        return False
    components_relations.append(new_relation)
    return True


def find_relations(child_name=None, child_type=None, parent_name=None):
    """
    通用查询函数（支持类型包含式查询）
    :param parent_name: 父组件
    :param child_name: 子组件
    :param child_type: 可传入单个类型或列表，查询包含关系
    """
    return [
        rel for rel in components_relations
        if (child_name is None or rel[0] == child_name) and
           (child_type is None or (
                   (isinstance(child_type, list) and all(ct in rel[1] for ct in child_type)) or
                   (not isinstance(child_type, list) and child_type in rel[1])
           )) and
           (parent_name is None or rel[2] == parent_name)
    ]


def has_relation(child_name=None, child_type=None, parent_name=None):
    """ 检查是否存在满足条件的条目 """
    return len(find_relations(child_name, child_type, parent_name)) > 0


# 快捷查询函数
def has_child(child_name, child_type):
    """ 检查是否存在包含指定类型的子组件 """
    return has_relation(child_name=child_name, child_type=child_type)


def has_parent(parent_name):
    """ 检查是否存在指定父组件 """
    return has_relation(parent_name=parent_name)

    # 定义处理规则：每个函数对应的参数提取方式和转换逻辑


def get_method_from_map(method_map, args,default_method):
    """
    解析method映射表。如果映射表里的handler是None，那么就说明是单参数，对应索引即可
    :param method_map: 方法的参数映射表
    :param args: 函数的完整形参
    :param default_method:默认方法
    :return:
    """
    if method_map['mapping']['handler'] is None:
        params =[args[i] for i in method_map['mapping']['index']]
        return method_map['mapping'].get(params, default_method)
    else:
        # 映射表里的method的第一个参数要确保是判断结果，结果若为None，那么就使用默认method
        result,method = method_map['mapping']['handler'](args,method_map['mapping'])
        if result is None:
            return default_method
        return method

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
            parts[-3] + check_default_arguments_2(parts[-2:], '0')
            if parts[-3] in mapping
            else check_default_arguments_2(parts[-2:], '0'),
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
        # 参数缺省映射表，由专门的函数进行重载解析，最后返回参数列表。如果无参就返回None
        'args_map': ['NULL','NULL'],
        'method_map': {
            # pos确定哪些参数决定改变method
            'index': [1],
            'mapping': {
                '': {'method': 'src', 'args': "{}"},
            },
            # 多参数处理时，需要自定一个lambda，用于返回处理的结果。单参数，直接默认从映射表里找
            # 第一
            'handler':None
        },
        # method给出的默认值的意思是：Flase:形参全缺省，那么函数调用可省略，True：形参全缺省，函数调用也不能省略。
        'method_processor': lambda args,method_map: (
            True,
            get_method_from_map(method_map, args, 'src')

       )
    }
}






def omit_parameters(args, default_values):
    """
    根据C++重载规则处理参数列表，省略符合条件的参数。

    :param args: 传入的参数列表
    :param default_values: 默认值列表，对应参数列表中最后几个参数的默认值
    :return: 处理后的参数列表和判断值（True表示有参数未省略，False表示全省略）
    """
    assert len(args) >= len(default_values), "参数列表长度必须大于等于默认值列表长度"

    default_count = len(default_values)  # 确保这是一个整数
    t = 0
    # 从后往前逐个检查参数是否符合默认值
    for i in range(default_count):
        pos = -(i + 1)
        if args[pos] == default_values[i]:
            t += 1
        else:
            break
    # 生成处理后的参数列表
    processed_args = args[:len(args) - t] if t > 0 else list(args)  # 使用list()确保返回一个新的列表
    # 判断是否所有可省略参数都符合条件
    all_omitted = t == len(default_values)
    return not all_omitted, processed_args


def convert_function_args(func_name, args):
    """
    转换函数与形参
    :param func_name:
    :param args:为避免影响后续扩展，这里传输整个形参表
    :return: 如果处理正常，就会返回正确的值
    """
    # 处理样式类函数
    # 1，根据函数名选择具体的规则。
    # 2，找到字典里的函数名后，根据形参的不同，选择对应的method或者省略特定的参数。
    # 3，函数名只需要匹配在不在即可，也就是 if xxx in
    # 4,每个函数名规则都有默认的method，形参也默认，只有当形参匹配时才重新设置method和参数
    # 5,也就是映射表，映射的是参数与函数名的对应的关系，传入的是参数列表，
    # 6，映射表改变的是method，参数值改变的是它本身。
    # 7，所以需要两个处理器，每个处理器除了传入参数外，还需要传入映射表
    # 如果存在映射表，则使用映射表,不存在的话，处理器会默认
    # 专门对参数的处理会有缺省，method则不需要。也就是说会缺省也是默认值的体现。
    # 8，缺省采取从右往左，按照优先级依次缺省。那么参数的缺省可以专门写一个函数来处理，字典里存放的就是缺省默认值
    # 9，整个表达式省略的情况：参数全部默认并且method没有特殊处理，那么就省略。那么这两个函数可以分别给出个结果
    # 10,method给出的默认值的意思是：Flase:形参全缺省，那么函数调用可省略，True：形参全缺省，函数调用也不能省略。
    for funcs, config in function_handlers.items():
        # 匹配函数规则
        if func_name in funcs:
            # 处理参数规则
            args_result, final_args = omit_parameters(args, config['args_map'])
            method_result, final_method = config['method_processor'](args, config['method_map'])
            if args_result or method_result:
                return f"{final_method}({', '.join(final_args)})"
            else:
                return None

    # 其他未处理情况
    raise NotImplementedError(f"Function '{func_name}' not implemented.")
    # return None


# --------------------------------分步处理代码块------------------------------------

# 处理样式块,已经包含里样式的正确代码，不用在前面加上组件名
def convert_style_calls(blocks):
    converted = []
    default_config = [
        ['border_width', "0", 'LV_PART_MAIN|LV_STATE_DEFAULT'],
        ['text_color', 'lv_color_hex(0xffffff)', 'LV_PART_MAIN|LV_STATE_DEFAULT'],
        ['text_opa', "255", 'LV_PART_MAIN|LV_STATE_DEFAULT'],
        ['text_line_space', "0", 'LV_PART_MAIN|LV_STATE_DEFAULT'],
        ['text_align', 'LV_TEXT_ALIGN_CENTER', 'LV_PART_MAIN|LV_STATE_DEFAULT'],
        ['bg_opa', "255", 'LV_PART_MAIN|LV_STATE_DEFAULT'],
        ['pad_top', "0", 'LV_PART_MAIN|LV_STATE_DEFAULT'],
        ['pad_right', "0", 'LV_PART_MAIN|LV_STATE_DEFAULT'],
        ['pad_bottom', "0", 'LV_PART_MAIN|LV_STATE_DEFAULT'],
        ['pad_left', "0", 'LV_PART_MAIN|LV_STATE_DEFAULT'],
        ['shadow_width', "0", 'LV_PART_MAIN|LV_STATE_DEFAULT']
    ]
    for block in blocks:
        # 给每一块代码分成若干行
        lines = block.split('\n')
        for line in lines:
            line = line.strip()
            if not line:
                continue

            # 这里能做到这么简洁是因为lvgl 9.2的所有样式API都只有三个参数，并且前缀相同
            # 匹配对应函数 第二个是非贪婪捕获，第三个是贪婪捕获，不过在这里都差不多
            match = re.match(r'lv_obj_set_style_(\w+)\(ui->\w+,\s*(.+?),\s*(.*)\);', line)
            if match:
                # 定义一个函数映射表
                prop = match.group(1)
                value = match.group(2)
                state = match.group(3)

                # 如果在默认配置表里找到了对应函数，那么就对照默认配置修改
                index = next((i for i, item in enumerate(default_config) if item[0] == prop), -1)
                if index != -1:
                    # 处理状态，如果是默认状态，那么就不添加
                    state = "" if state == 'LV_PART_MAIN|LV_STATE_DEFAULT' else f", {state}"
                    value = "" if value == default_config[index][1] and state == "" else f"{value}"
                    if state == "" and value == "":
                        # 如果都为默认值，那么就跳过这次处理
                        continue
                else:
                    # 暂时不与上面的合并，因为以后可能会有新的样式
                    state = "" if state == 'LV_PART_MAIN|LV_STATE_DEFAULT' else f", {state}"

                # 每个样式前加上一个Tab，是为了与init对齐
                converted.append(f"\t.{prop}({value}{state})")

    return converted


def process_init_function(init_lines, component_name, widget_info):
    # 处理初始化代码块（不包含创建）
    """
    :param widget_info:
    :param init_lines: 包含了各种初始化代码的行
    :param component_name: 没有前缀的组件名
    :note: 把各个初始化行解析并转为链式调用
    :return 返回的是处理后的列表
    """
    # 处理初始化代码的主逻辑
    init_code = []
    child_relations = []  # 子组件名，子组件前缀，父组件名
    previous_component_name = ''  # 存储上一个组件的名称，用于判断链式调用
    for line in init_lines:
        # 跳过创建函数和空行
        if not line.strip():
            continue

        # 词法解析，获取变量名、函数名（不为空）和参数表
        var_name, func_name, args = parse_function_line(line)
        if func_name is None:
            continue

        # 初始化代码有两种,一种是创建组件，一种是初始化组件
        if var_name is None:
            # 初始化组件
            pre_name = ''
            current_component_name = extract_right_variable_name(args[0])
            # 如果组件名称相同，那么就继续添加参数
            if previous_component_name != current_component_name:
                # 把处理过的上行代码添加分号,隔离开
                init_code[-1] = init_code[-1] + ';'
                # 查询当前组件的组件信息
                current_widget_info = find_relations(child_name=current_component_name)[1]
                pre_name = current_widget_info[0] + '_' + current_component_name
            function_info = convert_function_args(func_name, args)
            init_code.append(f"{pre_name}.{function_info}")
            previous_component_name = current_component_name

        else:
            # 创建子组件，需要先分析组件类型和父对象
            current_widget_info = get_widget_type(line)
            child_var_name = extract_right_variable_name(var_name)
            parent_var_name = extract_right_variable_name(args[0])
            new_child_var_name = current_widget_info[0] + '_' + child_var_name
            # 定义组件代码
            widgets_ns.append(f"{current_widget_info[1]}  {new_child_var_name};")
            # 把组件加入关系字典，以没有前缀的形式便于查找
            if not add_component_relations(child_var_name, current_widget_info, parent_var_name):
                raise Exception("组件关系错误")
            # 把处理过的上行代码添加分号,隔离开
            init_code[-1] = init_code[-1] + ';'
            # 添加此时的代码
            new_parent_var_name = find_relations(child_name=parent_var_name)[1][0] + '_' + parent_var_name
            init_code.append(f"{new_child_var_name}.init({new_parent_var_name})")
            previous_component_name = child_var_name  # 存档

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


#
"""
@ component_name 
@ create_line 创建组件的代码行
@ init_lines 初始化组件属性的代码行
@ style_block 样式代码块
"""


def process_component_block(component_name, create_line, init_lines, style_block):
    """
    处理组件代码块
    :param component_name: 从注释里包含的组件名称
    :param create_line: 创建组件的代码行
    :param init_lines: 初始化代码行
    :param style_block: 样式代码块
    :return: 组件字典
    """
    # 词法解析，获取变量名、函数名（不为空）和参数表
    _, func_name, args = parse_function_line(create_line)
    widget_info = get_widget_type(create_line)
    # 添加组件关系
    if not add_component_relations(component_name, widget_info, args[0]):
        # 存在预期外的重复组件关系
        raise ValueError("Failed to add component relations")

    # 合成新组件名
    var_name = widget_info[0] + '_' + component_name

    # 处理初始化行，获取位置大小等信息，如果遇到create就跳过（因为链式调用里会把这个包含进函数里）
    init_chain = process_init_function(init_lines, component_name, widget_info)

    # 处理样式块，样式块是一段段样式初始化代码
    style_code = convert_style_calls(style_block)

    return {
        "var_name": var_name,
        "init_chain": init_chain,
        "style_code": style_code,
        "widget_info": widget_info
    }


# --------------------------------处理屏幕和组件代码块------------------------------------
def process_main_screen_blocks(blocks):
    """
    处理主屏幕的代码块
    @param blocks: 块代码
    @notes: 添加进了components
    """
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
                if prop == "bg_opa" and value == "255":
                    continue

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
            "widget_type": ''
        }
        components.append(component)


def process_component_blocks(blocks):
    """
    处理组件代码块，把初步分割的代码块进行更细致的处理
    @param blocks: 块代码
    @notes: 添加进了components
    """
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
        if comp['var_name'] == "scr":
            screen_code = []
            screen_code.extend(comp['style_code'])
            if screen_code:
                screen_code[0] = "scr" + screen_code[0]
                screen_code[-1] += ";\n"
            screen_ns.extend(screen_code)

        else:
            # 组件定义代码
            widgets_ns.append(f"{comp['widget_type']}  {comp['var_name']};")

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
    # 把主屏幕放入组件中，并且把父组件名设置为''来标记
    components_relations.append([project_name, [], '', []])
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
