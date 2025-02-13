#!/usr/bin/env python3
import argparse
import glob
import os
import re

# 我应该分成两个样式处理表，一个处理屏幕的，比较简单，另一个处理其他组件的文件。
# 因为处理屏幕需要把名称变成scr，其他组件则是真名
# 我应该在处理样式表时定义一个映射表，把组件名映射为对应的样式名，并且把一些函数整合到一起

# 组件定义代码块
widgets_ns = []
# 界面初始化代码块
screen_ns = []

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


# ------------------------文本处理------------------------------

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
    # raise ValueError(f"Could not find prefix for {create_line}")


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





# 组状态存储
function_handlers = {
    'lv_obj_set_size': {
        # 不存在缺省参数
        'args_map': [],
        'method_map': {
            'index': [],
            'mapping': {},
            'type': '',
            'default': 'size',
            'handler': None
        }
    },
    'lv_obj_set_pos': {
        # 不存在缺省参数
        'args_map': [],
        'method_map': {
            'index': [],
            'mapping': {},
            'type': '',
            'default': 'pos',
            'handler': None
        }
    },
    'lv_obj_add_flag': {
        # 不存在缺省参数
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
    'lv_obj_align': {
        # 有两个缺省参数
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
    'lv_obj_set_scrollbar_mode': {
        'args_map': ['LV_SCROLLBAR_MODE_OFF'],
        'method_map': {
            'index': [],
            'mapping': {},
            'type': None,
            'default': 'scrollbar_mode',
            'handler': None
        }
    },
    # 图像按钮特殊处理
    'lv_imagebutton_set_src': {
        # 参数缺省映射表，由专门的函数进行重载解析，最后返回参数列表。如果无参就返回None
        'args_map': ['NULL', 'NULL'],
        'method_map': {
            # 确定哪些参数能决定改变method，单映射的情况下，handler为None即可
            'index': [1],
            # 映射表可以为空，映射之后这个参数就要去掉，因为我的目的是简化
            'mapping': {
                'LV_IMAGEBUTTON_STATE_RELEASED': 'released_src',
            },
            # 类型为None表示参数全缺省即可免去调用，不为None表示不缺省就省略。
            'type': '',
            # method默认在映射表里去找，如果找不到就用默认值
            'default': 'src',
            # 单参数或者多参数处理时，需要自定一个lambda，用于返回处理的结果。
            # 输入的参数是索引(或者处理过的参数)、参数表和mapping，需要返回一个判断结果（以None为标志）和一个最终处理的函数名和参数表
            # 事实上，更复杂的的需求不可能依靠lambda来完成，但是现在并没有遇到情况，就先用lambda代替
            'handler': None
        },
    }
}


def get_method_from_map(all_omitted, args, method_map):
    """
    解析method映射表，返回是否省略调用、方法名和参数列表。
    :param all_omitted: 是否所有参数都被省略。
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
    转换函数与形参:输入函数名和完整形参表，会返回生成好的C++函数
    :param func_name:
    :param args:为避免影响后续扩展，这里传输整个形参表
    :return: 如果处理正常，就会返回正确的C++函数代码（前面带点）
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
            args_result, handle_args = omit_parameters(args, config['args_map'])
            final_result, final_method, final_args = get_method_from_map(args_result, handle_args, config['method_map'])
            # 判断是否该省略调用
            if final_result:
                return None
            else:
                return f".{final_method}({', '.join(final_args[1:])})"

    # 其他未处理情况
    # TODO 暂时不处理，因为目前没有遇到需要处理这种情况
    # raise NotImplementedError(f"Function '{func_name}' not implemented.")
    return None


# --------------------------------分步处理代码块------------------------------------

# 处理样式块,已经包含里样式的正确代码，不用在前面加上组件名
def convert_style_calls(func_name, args):
    """
    转换样式代码
    :param func_name:
    :param args:
    :return: converted(string) 返回的是转换好的样式代码，如果不需要那么就返回None
    """
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
    if index != -1:

        # 处理状态，如果是默认状态，那么就不添加
        state = "" if state == 'LV_PART_MAIN|LV_STATE_DEFAULT' else f", {state}"
        value = "" if value == default_config[index][1] and state == "" else f"{value}"
        if state == "" and value == "":
            # 如果都为默认值，那么就跳过这次处理
            return None
    else:
        # 暂时不与上面的合并，因为以后可能会有新的样式
        state = "" if state == 'LV_PART_MAIN|LV_STATE_DEFAULT' else f", {state}"

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
    widgets_init_code.append('\n\t\t'.join(screen_init_chain))

    # 处理其他组件
    for widget in widgets:
        widget_name = widget[0]
        function_list = widget[1]
        # 链式调用：创建组件的代码
        widget_relation = find_relations(child_name=widget_name)
        widget_info = widget_relation[1]
        parent_name = widget_relation[2]
        widget_init_chain = [f'\n\t{widget_name}.init({parent_name})']
        # 组件定义
        widget_name_var_name = widget_info[0] + '_' + widget_name
        widgets_define.append(f'\t{widget_info[1]} {widget_name_var_name}')
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
                continue
            else:
                widget_init_chain.append(func_code)

        # 拼接为组件初始化代码
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

    # 【定位文件】：寻找目标文件
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
