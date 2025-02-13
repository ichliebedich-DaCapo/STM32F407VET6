#!/usr/bin/env python3
import argparse
import glob
import os
import re

# 组件代码块
widgets = []
# 组件的关系, 格式为 [['子组件名', [子组件类型], '父组件名'], ...]
widgets_relations = []
# 组件定义代码块，格式为 ['Component xxx','']
widgets_define = []


# --------------------------------预处理------------------------------------

# 寻找setup_scr_*函数
def parse_setup_function(content):
    pattern = r'void setup_scr_(\w+)\(lv_ui \*ui\)\s*{([^}]+)}'
    match = re.search(pattern, content, re.DOTALL)
    if not match:
        raise ValueError("Could not find setup_scr_* function")
    return match.group(1), match.group(2).strip()


# ---------------------------------工具函数----------------------------------------
# -----------------------文本处理--------------------------

# 输入组件创建代码块，返回组件前缀和组件类型
def get_widget_info(create_line):
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
        "lv_button_create": ["btn", 'Button'],
        "lv_checkbox_create": ['chekcbox', 'CheckBox'],
    }
    for func, widget_info in widget_map.items():
        if func in create_line:
            return widget_info

    # 报错，确保能及时发现有未入库的函数，以便及时添加
    # TODO: 这里需要进一步检查，把所有匹配上的都添加进去
    raise ValueError(f"Could not find prefix for {create_line}")


# 词法解析,解析代码行的赋值函数语句
# 返回的是变量名，函数名，参数列表（已去除两端空白字符）
def parse_function_line(code_line):
    """
    词法解析，把代码行解析为【变量名（可选）】、【函数名】和【形参表（已去除两端空白字符，并分割）】
    :param code_line: 去除换行的代码行
    :return: (var, func_name, args)，未匹配到会返回None,var是全匹配，包含ui->
    """

    # 正则表达式模式
    pattern = re.compile(
        r'^\s*'  # 起始空白
        r'(?:([^=\s]+)\s*=\s*)?'  # 变量名（可选）
        r'([a-zA-Z_]\w*)'  # 函数名
        r'\s*\(\s*'  # 左括号
        r'([^()]*(?:\([^()]*\))*[^()]*'  # 形参表（允许一层括号嵌套）
        r')\)\s*;'  # 右括号和分号
        r'\s*$'  # 结束空白
    )

    # 使用正则表达式进行匹配
    match = pattern.match(code_line)
    if match:
        var_name, func_name, params = match.groups()
        # 清理形参表中的多余空格
        params = params.strip() if params else ''
        params = params.split(',') if params else []
        params = [p.strip() for p in params]
        # print(f'code_line:|{code_line}|')
        # print(f"var:{var_name}  func:{func_name}  args:{params}")
        return var_name, func_name, params
    else:
        print(f"code_line:|{code_line}|")
        raise ValueError("Could not find function line")
        # 如果没有匹配到，返回错误提示


def extract_comp_name_from_var(var):
    """
    从变量名字符串中提取箭头右边的部分。
    :param var: 包含箭头的变量名字符串
    :return: 箭头右边的变量名部分
    """
    # 查找箭头的位置
    arrow_index = var.find('->')

    if arrow_index != -1:
        # 如果找到了箭头，返回箭头右边的部分
        return var[arrow_index + 2:]
    else:
        # 如果没有找到箭头，返回原始字符串
        return var


def extract_c_code_lines(input_text):
    """
    把文本里的代码提取出一行代码
    :param input_text: 
    :return: 
    """
    code_lines = []
    lines = input_text.splitlines()
    for line in lines:
        stripped_line = line.strip()
        if not stripped_line:
            continue  # 跳过空行
        # 分割行内注释，取注释前的代码部分
        code_part = stripped_line.split('//', 1)[0].strip()
        if code_part:
            code_lines.append(code_part)
    return code_lines


# -----------------------------与组件有关系--------------------------------
def check_widget_exists(widget_name):
    """检查组件是否已存在
    Args:
        widget_name (str): 要检查的组件名称
    Returns:
        bool: 是否存在
    """
    return any(widget[0] == widget_name for widget in widgets)


def add_function_to_existing(widget_name, func_name, args):
    """添加函数到已有组件
    Args:
        widget_name (str): 目标组件名称
        func_name (str): 要添加的函数名称
        args (list): 函数参数列表
    """
    for widget in widgets:
        if widget[0] == widget_name:
            widget[1].append([func_name, args.copy()])  # 使用拷贝避免引用问题
            break


def create_new_widget(widget_name, func_name, args):
    """创建新组件结构
    Args:
        widget_name (str): 组件名称
        func_name (str): 函数名称
        args (list): 函数参数列表
    Returns:
        list: 新组件结构
    """
    return [widget_name, [[func_name, args.copy()]]]


def add_widget_function(widget_name, func_name, args):
    """添加组件函数
    实现逻辑：
    1. 存在组件 -> 追加函数
    2. 不存在组件 -> 创建新组件
    Args:
        widget_name (str): 组件名称
        func_name (str): 函数名称
        args (list): 函数参数列表
    """
    if check_widget_exists(widget_name):
        add_function_to_existing(widget_name, func_name, args)
    else:
        new_widget = create_new_widget(widget_name, func_name, args)
        widgets.append(new_widget)


def find_widget(widget_name):
    """
    查找指定组件
    :param widget_name:
    :return: 找到就返回列表，找不到就返回None
    """
    for item in widgets:
        if item[0] == widget_name:
            return item
    return None


def remove_widget(widget_name):
    """
    删除指定组件
    :param widget_name: 组件名称
    :return: 删除后组件的剩余数量
    """
    # 使用列表解析来创建一个新的列表，排除掉要删除的元素
    original_length = len(widgets)
    widgets[:] = [item for item in widgets if item[0] != widget_name]
    removed_count = original_length - len(widgets)
    return removed_count  # 返回已删除项目的数量


# ------------------------------与组件关系有关------------------------
def add_widget_relations(child_name, child_info, parent_name):
    """
    添加组件关系（支持类型列表）
    :param child_name: 子组件名
    :param child_info: 子组件类型列表（如 ["UI", "Button"]）
    :param parent_name: 父组件名
    :return: True表示添加成功，False表示已存在
    """
    # 自动包装单元素类型为列表（可选）
    if not isinstance(child_info, list):
        child_info = [child_info]

    new_relation = [child_name, child_info, parent_name]
    if new_relation in widgets_relations:
        return False
    widgets_relations.append(new_relation)
    return True


def find_relations(child_name=None):
    """
    通用查询函数（支持类型包含式查询）
    :param child_name: 子组件（可选）
    :return 返回的是[child_name,child_type,parent_name]类型
    """
    for rel in widgets_relations:
        if child_name is None or rel[0] == child_name:
            return rel


# 函数映射表
function_handlers = {
    # 图像按钮
    'lv_imagebutton_set_src': {
        # 参数缺省映射表，由专门的函数进行重载解析，最后返回参数列表。如果无参就返回None
        'args_map': ['NULL', 'NULL'],
        'method_map': {
            # 确定哪些参数能决定改变method，单映射的情况下，handler为None即可
            'index': [1],
            # 映射表可以为空，映射之后这个参数就要去掉，因为我的目的是简化
            'mapping': {
                'LV_IMAGEBUTTON_STATE_RELEASED': 'released_src',
                'LV_IMAGEBUTTON_STATE_PRESSED': 'pressed_src',
                'LV_IMAGEBUTTON_STATE_CHECKED_RELEASED': 'checked_released_src',
                'LV_IMAGEBUTTON_STATE_CHECKED_PRESSED': 'checked_pressed_src',
            },
            # 类型为None表示参数全缺省即可免去调用，不为None表示不缺省就省略（这个优先级是高于args_map为空的）。
            'type': '',
            # method默认在映射表里去找，如果找不到就用默认值
            'default': 'src',
            # 单参数可以自定一个lambda，多参数处理时，必须要自定一个lambda，用于返回处理的结果。
            # 输入的参数是索引(或者处理过的参数)、参数表和mapping，需要返回一个判断结果（以None为标志）和一个最终处理的函数名和参数表
            # 事实上，更复杂的的需求不可能依靠lambda来完成，但是现在并没有遇到情况，就先用lambda代替
            'handler': None
        },
    },
    # 不存在缺省参数
    'lv_obj_set_size': {
        'args_map': [],
        'method_map': {
            'index': [],
            'mapping': {},
            'type': '',
            'default': 'size',
            'handler': None
        }
    },
    # 不存在缺省参数
    'lv_obj_set_pos': {
        'args_map': [],
        'method_map': {
            'index': [],
            'mapping': {},
            'type': '',
            'default': 'pos',
            'handler': None
        }
    },
    # 不存在缺省参数
    'lv_obj_set_width': {
        'args_map': [],
        'method_map': {
            'index': [],
            'mapping': {},
            'type': '',
            'default': 'width',
            'handler': None
        }
    },
    # 不存在缺省参数
    'lv_obj_add_flag': {
        'args_map': [],
        'method_map': {
            'index': [1],
            'mapping': {
                'LV_OBJ_FLAG_CHECKABLE': 'checkable',
                'LV_OBJ_FLAG_HIDDEN': 'hidden',
                'LV_FLAG_SCROLLABLE': 'scrollable',
            },
            'type': '',
            'default': 'add_flag',
            'handler': None
        }
    },
    # 全缺省不可免去调用
    'lv_obj_align': {
        'args_map': [0, 0],
        'method_map': {
            'index': [1],
            'mapping': {
                'LV_ALIGN_CENTER': 'center',
                'LV_ALIGN_RIGHT': 'right',
                'LV_FLAG_LEFT': 'left',
            },
            'type': '',
            'default': 'align',
            'handler': None
        }
    },
    # 全缺省可免调用
    'lv_obj_set_scrollbar_mode': {
        'args_map': ['LV_SCROLLBAR_MODE_AUTO'],
        'method_map': {
            'index': [],
            'mapping': {},
            'type': None,
            'default': 'scrollbar_mode',
            'handler': None
        }
    },
    # 不存在缺省参数
    'lv_label_set_text': {
        'args_map': [],
        'method_map': {
            'index': [],
            'mapping': {},
            'type': '',
            'default': 'text',
            'handler': None
        }
    },
    # 不存在缺省参数
    'lv_checkbox_set_text': {
        'args_map': [],
        'method_map': {
            'index': [],
            'mapping': {},
            'type': '',
            'default': 'text',
            'handler': None
        }
    },
    # 全缺省可免调用
    'lv_label_set_long_mode': {
        'args_map': ['LV_LABEL_LONG_WRAP'],
        'method_map': {
            'index': [],
            'mapping': {},
            'type': None,
            'default': 'long_mode',
            'handler': None
        }
    },
    # 不存在缺省参数
    'lv_image_set_src': {
        'args_map': [],
        'method_map': {
            'index': [],
            'mapping': {},
            'type': '',
            'default': 'src',
            'handler': None
        }
    },
    # 不存在缺省参数
    'lv_image_set_pivot': {
        'args_map': [],
        'method_map': {
            'index': [],
            'mapping': {},
            'type': '',
            'default': 'pivot',
            'handler': None
        }
    },
    # 全缺省可免调用
    'lv_image_set_rotation': {
        'args_map': [0],
        'method_map': {
            'index': [],
            'mapping': {},
            'type': None,
            'default': 'rotation',
            'handler': None
        }
    },
}


def get_method_from_map(all_omitted, args, method_map):
    """
    解析method映射表，返回是否省略调用、方法名和参数列表。
    :param all_omitted: 是否所有参数都被省略(True/False)。
    :param args: 处理后的参数列表。
    :param method_map: 方法的参数映射表，包含index、mapping、handler等键。
    :return: (omit_call, method_name, processed_args)
            第一个参数为True表示可以跳过,最后一个参数是包含了第一个参数，即组件自身，需要主动剔除掉
    """
    # 如果全缺省并且类型为None，那么就跳过函数调用
    if all_omitted and method_map.get('type') is None:
        return True, None, []

    # 获取默认method和handler
    default_method = method_map.get('default', '')
    handler = method_map.get('handler')

    if handler is None:
        # 如果handler不存在
        indices = method_map.get('index', [])
        if not indices:
            # 如果是无参数映射，直接返回默认方法和参数
            return False, default_method, args

        # 处理单参数映射，取第一个索引
        index = indices[0]
        if index >= len(args):
            # 单参数映射超出范围,说明映射表需要修正
            raise ValueError(f"Invalid index {index} for method mapping")

        # 进行单参数映射
        param = args[index]
        mapping = method_map.get('mapping', {})
        method = mapping.get(param, default_method)

        if method == default_method:
            return False, default_method, args
        else:
            # 存在映射，则删除指定元素
            new_args = list(args)
            del new_args[index]
            return False, method, new_args
    else:
        # 如果handler存在（单参数映射或者多参数映射）
        indices = method_map.get('index', [])
        params = [args[i] for i in indices if i < len(args)]
        # 使用映射表里的lambda，输入索引(或者处理过的参数)、参数表和mapping，返回结果、method和参数
        # 映射表里的method的第一个参数要确保是判断结果，结果若为None，那么就使用默认method
        result, method, final_args = handler(params, args, method_map.get('mapping', {}))
        if result is None:
            method = default_method
        return False, method, final_args


def omit_parameters(args, default_values):
    """
    根据C++重载规则处理参数列表，省略符合条件的参数。

    :param args: 传入的参数列表
    :param default_values: 默认值列表，对应参数列表中最后几个参数的默认值
    :return: 处理后的参数列表和判断值（True表示全省略，False表示有参数未省略）
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
    return all_omitted, processed_args


def convert_function_args(func_name, args):
    """
    转换函数与形参:输入函数名和完整形参表，会返回生成好的C++函数
    :param func_name:
    :param args:为避免影响后续扩展，这里传输整个形参表
    :return: 如果处理正常，就会返回正确的C++函数代码（前面带点）
    """
    for funcs, config in function_handlers.items():
        # 匹配函数规则
        if func_name in funcs:
            # 处理参数规则
            args_result, handle_args = omit_parameters(args, config['args_map'])
            final_result, final_method, final_args = get_method_from_map(args_result, handle_args, config['method_map'])
            # 判断是否该省略调用
            if final_result:
                return None
            else:
                return f".{final_method}({', '.join(final_args[1:])})"

    # 其他未处理情况
    # TODO 暂时不处理，因为目前没有遇到需要处理这种情况
    # 出现这个错误，说明method映射表需要补充，在【function_handlers】字典里补充对应的{func_name}
    raise NotImplementedError(f"Function '{func_name}' not implemented.")
    # return None


# --------------------------------分步处理代码块------------------------------------


# 处理样式块,已经包含里样式的正确代码，不用在前面加上组件名
def convert_style_calls(func_name, args):
    """
    转换样式代码
    :param func_name:
    :param args:
    :return: converted(string) 返回的是转换好的样式代码，如果不需要那么就返回None
    """
    # 每个列表都有2~3个元素，左边是匹配的函数名（相当于键），第二个元素是默认值（为''表示没有），第三个元素是函数的简写（如果有的话）
    default_config = [
        ['border_width', "0"],
        ['text_color', 'lv_color_hex(0xffffff)'],
        ['text_opa', "255"],
        ['text_font', '', 'font'],
        ['text_line_space', "0"],
        ['text_align', 'LV_TEXT_ALIGN_CENTER'],
        ['bg_opa', "255"],
        ['pad_top', "0"],
        ['pad_right', "0"],
        ['pad_bottom', "0"],
        ['pad_left', "0"],
        ['shadow_width', "0"],
        ['image_recolor_opa','255'],
        ['image_opa','255'],
        ['text_letter_space','0','letter_space']
    ]

    def list_get2(lst, default=None):
        """
        类似于字典的get方法，用于列表。这里默认去第三个元素
        :param lst: 要检查的列表
        :param default: 如果索引超出范围时返回的默认值
        :return: 列表中指定索引处的元素或默认值
        """
        try:
            return lst[2]
        except IndexError:
            return default

    # 这里能做到这么简洁是因为lvgl 9.2的所有样式API都只有三个参数，并且前缀相同
    # 匹配对应函数 第二个是非贪婪捕获，第三个是贪婪捕获，不过在这里都差不多
    match = re.match(r'lv_obj_set_style_(\w+)', func_name)
    if match is None:
        # 不应该出现的情况
        raise NotImplementedError(f"Function '{func_name}' not implemented.")

    prop = match.group(1)
    value = args[1]
    state = args[2]


    # 如果在默认配置表里找到了对应函数，那么就对照默认配置修改
    index = next((i for i, item in enumerate(default_config) if item[0] == prop), -1)
    prop = list_get2(default_config[index], prop)  # 从默认配置里看看有没有第三个元素（简写）
    print(f"prop:{args[0]} {match.group(1)}---{prop}")
    # 处理状态，如果是默认状态，那么就不添加
    state = "" if state == 'LV_PART_MAIN|LV_STATE_DEFAULT' else f", {state}"
    if index != -1:
        value = "" if value == default_config[index][1] and state == "" else f"{value}"
        if state == "" and value == "":
            # 如果都为默认值，那么就跳过这次处理
            return None

    # 每个样式前加上一个Tab，是为了与init对齐
    converted = f".{prop}({value}{state})"

    return converted


# --------------------------------解析代码------------------------------------
# -------------处理代码行----------------
def process_code_lines(code_lines):
    """
    处理代码行。把代码行里的信息全部存储到初始化信息表和关系表里
    :param code_lines: 已经做过处理，每行只包含代码行
    :note 向widget里添加['widget_name',[[func,[args]],……]]。其中组件名是已经去除ui->
    """
    for line in code_lines:
        # 词法解析，获取变量名、函数名（不为空）和参数表
        var, func_name, args = parse_function_line(line)
        # 剔除无关函数
        if 'events_init' in func_name:
            continue

        if var is None:
            # 初始化代码行：把组件信息提取到widgets里
            # lv_obj_set_scrollbar_mode(ui->blueCounter_cont_1, LV_SCROLLBAR_MODE_OFF);
            # ①先获取组件名称
            widget_name = extract_comp_name_from_var(args[0])
            # ②存储初始化信息  [['widget_name',['func_name',[args]],……],……]
            add_widget_function(widget_name, func_name, args)
        else:
            # 创建组件代码行：组件关系提取到widgets_relations里
            # ui->blueCounter_cont_1 = lv_obj_create(ui->blueCounter);
            # ①先获取父子组件名称
            child_name = extract_comp_name_from_var(var)
            parent_name = extract_comp_name_from_var(args[0])
            # ②获取组件类型
            widget_info = get_widget_info(line)
            # ③添加组件关系   [['child_name',[widget_info],'parent_name']]
            if not add_widget_relations(child_name, widget_info, parent_name):
                # 存在预期外的重复组件关系
                raise ValueError("Failed to add widget relations")


# ----------------解析组件信息-------------------
def iterate_widgets(screen_name):
    """遍历组件结构的通用函数
    # 1,链式调用需要一张表，组件在前面，然后init后跟父组件,接着不断往后添加到列表里
    # ['widget_name.init(parent_name)','.pos()','size()',]
    # 2,根据实际情况，组件名称前需要\n\t\t，两个Tab，而其他链式调用则需要三个Tab
    # 3,对widgets_define添加了组件定义的信息，但是没有把里面的代码通过'\n\t'连接
    Args:
        screen_name(string): 项目名称，这里是为了把屏幕名称替换为scr
    Returns:
        widgets_init_code(list):由一段段组件初始化代码组成,最后是需要通过'\n\t'进行拼接
    """
    widgets_init_code = []
    # 处理屏幕组件
    screen_widget = find_widget(screen_name)
    screen_func_list = screen_widget[1]
    screen_init_chain = []
    # 剔除屏幕组件
    remove_widget(screen_name)
    for scr_func_info in screen_func_list:
        func_name = scr_func_info[0]
        args = scr_func_info[1]
        if 'lv_obj_set_style' in func_name:
            # 如果是样式函数
            func_code = convert_style_calls(func_name, args)
            if func_code is None:
                continue
            screen_init_chain.append(func_code)
    # 给组件的第一个样式函数前加上scr
    screen_init_chain[0] = '\n\tscr' + screen_init_chain[0]
    screen_init_chain[-1] += ';'  # 末尾添加分号
    widgets_init_code.append('\n\t\t'.join(screen_init_chain))

    # 处理其他组件
    for widget in widgets:
        widget_name = widget[0]
        function_list = widget[1]
        # 链式调用：创建组件的代码
        widget_relation = find_relations(child_name=widget_name)
        widget_info = widget_relation[1]
        parent_name = widget_relation[2]
        # 转为合成名
        widget_name_var_name = widget_info[0] + '_' + widget_name
        if parent_name == screen_name:
            # 空即默认主屏幕
            parent_name_var_name = ''
        else:
            parent_relation = find_relations(child_name=parent_name)
            parent_widget_info = parent_relation[1]
            parent_name_var_name = parent_widget_info[0] + '_' + parent_name
        widget_init_chain = [f'\n\t{widget_name_var_name}.init({parent_name_var_name})']
        # 组件定义
        widgets_define.append(f'\t{widget_info[1]} {widget_name_var_name};')
        for func_info in function_list:
            func_name = func_info[0]
            args = func_info[1]

            # 获得处理过后的C++函数代码
            if 'lv_obj_set_style' in func_name:
                # 如果是样式函数
                func_code = convert_style_calls(func_name, args)
            else:
                func_code = convert_function_args(func_name, args)

            # 判断是否跳过调用
            if func_code is None:
                print(f'Skipped function: \t{extract_comp_name_from_var(args[0])}\t{func_name}')
                continue
            else:
                widget_init_chain.append(func_code)

        # 拼接为组件初始化代码
        widget_init_chain[-1] += ';'  # 末尾添加分号
        widgets_init_code.append('\n\t\t'.join(widget_init_chain))
    # 给定义组件代码的首元素添加\n
    widgets_define[0] = '\n' + widgets_define[0]
    return widgets_init_code


# ----------------------------------输出结果处理--------------------------------------------
def generate_output(project_name):
    """
    输出转换后的代码结果。
    ①初始化代码可以划分为几个组件块，每个组件块都办包含了自己的名称和若干代码行。
    ②组件的每行代码行都有解析器解析后存储到属于该组件的位置
    ③那么包含了所有组件信息的列表应该这样设计算。
    ④widgets [['widget_name',['.init(89,63)','pos(165,35)',……]],……]
    ⑤为了便于处理，应把初始化代码和样式定义代码分开，最后在合并到一起。或者说如果发现解析出的函数名含"lv_obj_style"
        那么就使用样式代码的处理方式，最后都穿插到widgets里。这样也便于后续自定义lvgl代码也能解析
    ⑥对于塞入初始化代码的块，其实可以不用直接转为字符串的形式，可以先用列表存储起来，即
        [['widget_name',[['func_name','args'],,……]],……]
        收集所有信息 → 统合信息（把一些函数和参数拼接） → 链接函数调用块（把包含函数信息的列表拼接为字符串）
         → 解析组件（把widgets解析为对应的代码，首行的组件名称与函数调用拼接在一起）→ 代码生成
    :param project_name:
    :return:
    """

    widgets_init_code = iterate_widgets(project_name)
    widgets_block = "\n".join(widgets_define)
    screen_block = "\n".join(widgets_init_code)

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

    # 【定位文件】：寻找目标文件(暂时只处理单个屏幕文件）
    target_files = glob.glob(os.path.join(args.path, "setup_scr_*.c"))
    if not target_files:
        raise FileNotFoundError("No setup files found")
    if len(target_files) > 1:
        raise ValueError("Multiple setup files found")

    # 打开目标文件并读取内容(默认编码为 utf-8)
    with open(target_files[0], 'r', encoding='utf-8') as f:
        content = f.read()

    # 【定位屏幕初始化代码】：定位 setup_scr_* 函数,并获取工程名和函数体
    project_name, func_body = parse_setup_function(content)
    # 把主屏幕放入组件中，并且把父组件名设置为''来标记
    widgets_relations.append([project_name, [], '', []])

    # 【提取代码行】：从setup_scr_*函数体代码提取出代码行
    code_lines = extract_c_code_lines(func_body)

    # 【处理代码行】：从每行代码里提取到组件初始化信息和组件关系
    process_code_lines(code_lines)

    # 【解析信息】：把组件信息解析为对应的代码，并输出
    output = generate_output(project_name)

    with open("ui.cpp", "w", encoding='utf-8') as f:
        f.write(output)


if __name__ == "__main__":
    # 在执行其他命令之前设置代码页为UTF-8
    os.system('chcp 65001')
    main()
