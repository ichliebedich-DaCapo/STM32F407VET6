#!/usr/bin/env python3
import argparse
import glob
import os
import re
import shutil
import textwrap
from datetime import datetime

import networkx as nx
from dataclasses import dataclass
from enum import Enum
from pathlib import Path
from typing import Dict, Tuple, List, Callable, Optional

# 组件代码块
widgets = []
# 组件的关系, 格式为 [['子组件名', [子组件类型], '父组件名'], ...]
widgets_relations = []
# 组件定义代码块，格式为 ['Component xxx','']
widgets_define = []
# 资源文件,格式为{'font':[],'image':[]}  fonts和images都是对应的名称
resources = {
    'font': [],
    'image': [],
}


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


# --------添加资源---------
def add_resource(resource_type: str, resource: str) -> None:
    """增强型资源添加函数
    Args:
        resource_type: 资源类型（必须与resources键名一致）
        resource: 资源标识符，自动去除&前缀
    """
    # 参数校验
    if resource_type not in resources:
        raise ValueError(f"Invalid resource type: {resource_type}. Valid types: {list(resources.keys())}")

    # 标准化处理(去除&前缀)
    cleaned = resource.lstrip('&')

    # 去重逻辑
    if cleaned not in resources[resource_type]:
        resources[resource_type].append(cleaned)


def extract_resource(func_name: str, args: list) -> None:
    """增强型资源提取器
    Args:
        func_name: LVGL API函数名
        args: 实际调用参数列表
    """
    resource_map = {
        'lv_obj_set_style_text_font': {
            'type': 'font',  # 保持与resources键名一致
            'index': 1
        },
        'lv_imagebutton_set_src': {
            'type': 'image',
            'index': 2
        },
        'lv_image_set_src': {
            'type': 'image',
            'index': 1
        }
    }

    # 查询资源映射
    if (info := resource_map.get(func_name)) is None:
        return

    # 索引有效性检查
    if info['index'] >= len(args):
        # 映射表里的索引有问题，赶紧修复
        raise IndexError(f"Warning: Invalid arg index {info['index']} for {func_name}")

        # 执行资源添加
    add_resource(info['type'], str(args[info['index']]))


# ------------------------------与组件关系有关------------------------
def add_widget_relations(child_name, child_info=None, parent_name=''):
    """
    添加组件关系（支持类型列表）
    :param child_name: 子组件名
    :param child_info: 子组件类型列表（如 ["UI", "Button"]）,None表示没有
    :param parent_name: 父组件名
    :return: True表示添加成功，False表示已存在
    """
    # 自动包装单元素类型为列表（可选）
    if child_info is None:
        child_info = []
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
# ①缺省表要注意缺省值是字符串类型
# ②如果缺省表为空，那么默认是全缺省，所以，如果你不想被省略，那么需要把type设为非None，比如''
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
        'args_map': ['0', '0'],
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
        'args_map': ['0'],
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
    # 默认值列表长度
    default_count = len(default_values)
    t = 0
    # 从后往前逐个检查参数是否符合默认值
    for i in range(default_count):
        pos = -(i + 1)
        if args[pos] == default_values[i]:
            t += 1
        else:
            break

    # 生成处理后的参数列表
    if t > 0:
        processed_args = args[:len(args) - t]
    else:
        processed_args = args  # 直接使用原列表，避免不必要的拷贝

    # 判断是否所有可省略参数都符合条件
    all_omitted = (t == default_count)
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
def convert_style_calls(func_name, args, is_static_cast=False, is_font_custom=False):
    """
    转换样式代码
    :param is_font_custom:
    :param is_static_cast: 是否强制转换（在C++中，两个enum类型做运算会警告）
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
        ['image_recolor_opa', '255'],
        ['image_opa', '255'],
        ['text_letter_space', '0', 'letter_space']
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

    def add_enum_casts(code):
        """
        自动为形如 LV_XXX | LV_YYY 的枚举组合添加类型转换
        示例输入：'LV_PART_MAIN|LV_STATE_PRESSED'
        示例输出：'(lv_style_flag_t)LV_PART_MAIN|(lv_style_flag_t)LV_STATE_PRESSED'
        """
        # 匹配LV_开头的标识符组合
        pattern = r'\b(LV_[A-Z0-9_]+\b)(?:\s*\|\s*(LV_[A-Z0-9_]+\b))+'

        def replace_match(match):
            parts = match.group(0).split('|')
            casted = [f'static_cast<int>({part.strip()})' for part in parts]
            return '|'.join(casted)

        return re.sub(pattern, replace_match, code)

    # 这里能做到这么简洁是因为lvgl 9.2的所有样式API都只有三个参数，并且前缀相同
    match = re.match(r'lv_obj_set_style_(\w+)', func_name)
    if match is None:
        # 不应该出现的情况
        raise NotImplementedError(f"Function '{func_name}' not implemented.")

    prop = match.group(1)
    value = args[1]
    state = args[2]

    # 如果在默认配置表里找到了对应函数，那么就对照默认配置修改
    index = next((i for i, item in enumerate(default_config) if item[0] == prop), -1)
    # 判断是否需要强制转换
    is_default_state = state == 'LV_PART_MAIN|LV_STATE_DEFAULT'
    if is_static_cast:
        state = add_enum_casts(state)
    # 处理状态，如果是默认状态，那么就不添加
    state = "" if is_default_state else f", {state}"
    if index != -1:
        prop = list_get2(default_config[index], prop)  # 从默认配置里看看有没有第三个元素（简写）
        value = "" if value == default_config[index][1] and state == "" else f"{value}"
        # 如果是自定义字体，那么就加上字体类型(一般情况下只有第一个参数)
        if is_font_custom and prop == 'font':
            value = value.replace("_font", "_customer_font", 1)
        # 如果都为默认值，那么就跳过这次处理
        if state == "" and value == "":
            return None

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
def iterate_widgets(screen_name, is_font_custom):
    """遍历组件结构的通用函数
    Note:
        # 1,链式调用需要一张表，组件在前面，然后init后跟父组件,接着不断往后添加到列表里
            ['widget_name.init(parent_name)','.pos()','size()',]
        # 2,根据实际情况，组件名称前需要'\n\t\t'，两个Tab，而其他链式调用则需要三个Tab
        # 3,对widgets_define添加了组件定义的信息，但是没有把里面的代码通过'\n\t'连接
    Args:
        screen_name(string): 项目名称，这里是为了把屏幕名称替换为scr
        is_font_custom(bool): 自定义字体
    Returns:
        widgets_init_code(list):由一段段组件初始化代码组成,最后是需要通过'\n\t'进行拼接
    """
    widgets_init_code = []
    # -------处理屏幕组件------
    screen_widget = find_widget(screen_name)
    screen_func_list = screen_widget[1]
    screen_init_chain = []
    # 剔除屏幕组件
    remove_widget(screen_name)
    for scr_func_info in screen_func_list:
        func_name = scr_func_info[0]
        args = scr_func_info[1]
        if 'lv_obj_set_style' in func_name:
            # 如果是样式函数（针对与主屏幕）
            func_code = convert_style_calls(func_name, args, is_font_custom=is_font_custom)
            if func_code is None:
                continue
            screen_init_chain.append(func_code)
    # 给组件的第一个样式函数前加上scr
    screen_init_chain[0] = '\t\tscr' + screen_init_chain[0]
    screen_init_chain[-1] += ';'  # 末尾添加分号
    widgets_init_code.append('\n\t\t\t'.join(screen_init_chain))

    # ---------------处理其他组件------------
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
        widget_init_chain = [f'\n\t\t{widget_name_var_name}.init({parent_name_var_name})']
        # 组件定义
        widgets_define.append(f'\t{widget_info[1]} {widget_name_var_name};')
        for func_info in function_list:
            func_name = func_info[0]
            args = func_info[1]

            # 获得处理过后的C++函数代码
            if 'lv_obj_set_style' in func_name:
                # 如果是样式函数
                func_code = convert_style_calls(func_name, args, is_static_cast=True, is_font_custom=is_font_custom)
            else:
                # 如果是普通函数
                func_code = convert_function_args(func_name, args)
            extract_resource(func_name, args)  # 提取字体、图片资源

            # 判断是否跳过调用
            if func_code is None:
                print(f'Skipped function: \t{extract_comp_name_from_var(args[0])}\t{func_name}')
                continue
            else:
                widget_init_chain.append(func_code)

        # 拼接为组件初始化代码
        widget_init_chain[-1] += ';'  # 末尾添加分号
        widgets_init_code.append('\n\t\t\t'.join(widget_init_chain))
    # 给定义组件代码的首元素添加\n
    # widgets_define[0] = '\n' + widgets_define[0]
    return widgets_init_code, widgets_define


# ---------------------------------------文件处理函数-------------------------------------------
def find_c_functions(func_name: str,
                     search_path: str,
                     file_name: str = "setup_scr_*.c") -> Tuple[Tuple[str, List[str], str], ...]:
    """搜索指定C文件并返回结构化函数信息

    Args:
        func_name: 支持通配符的函数名（如"set_src_*"）
        search_path: 文件搜索根路径
        file_name: 文件名匹配模式（原pattern参数）

    Returns:
        元组格式: (
            ("函数名", ["参数1类型 参数1", ...], "去花括号函数体"),
            ...
        )

    Raises:
        FileNotFoundError: 文件未找到时抛出
        ValueError: 多个文件匹配或未找到函数时抛出
    """
    target_file = Path(search_path).glob(file_name)
    matched_files = list(target_file)

    # 文件校验
    if len(matched_files) == 0:
        raise FileNotFoundError(f"No files match '{file_name}' in {search_path}")
    if len(matched_files) > 1:
        raise ValueError(f"Multiple files match '{file_name}': {matched_files}")

    # 读取文件内容
    with open(matched_files[0], 'r', encoding='utf-8') as f:
        content = f.read()

        # 未指定函数名时返回完整内容
    if not func_name:
        return (("full_content", [], content),)

    # 构建正则表达式模式
    name_pattern = re.escape(func_name).replace(r'\*', '.*').replace(r'\?', '.')
    func_regex = re.compile(
        r'^\s*((?:[\w_]+\s+)+?)'  # 返回类型 
        rf'({name_pattern})\s*'  # 函数名 
        r'\(([^)]*)\)\s*'  # 参数列表 
        r'\{',
        re.MULTILINE
    )

    functions = []
    for match in func_regex.finditer(content):
        # 提取函数信息
        func_name = match.group(2)
        params = [p.strip() for p in match.group(3).split(',') if p.strip()]

        # 提取函数体（去花括号）
        start = match.end()
        brace_level = 1
        end_pos = None

        for i in range(start, len(content)):
            if content[i] == '{':
                brace_level += 1
            elif content[i] == '}':
                brace_level -= 1

            if brace_level == 0:
                end_pos = i
                break

        if end_pos is None:
            continue  # 忽略不完整定义

        # 清理函数体格式
        func_body = content[start:end_pos].strip()
        func_body = re.sub(r'^\s*{ |\s*}$', '', func_body, flags=re.DOTALL).strip()

        functions.append((func_name, params, func_body))

    if not functions:
        raise ValueError(f"No functions matching '{func_name}' found")

    return tuple(functions)


def extract_screen_name(func_name: str) -> str:
    """
    从函数名里提取屏幕名称
    :param func_name:
    :return:
    """
    match = re.search(r'setup_scr_(\w+)', func_name)
    if match:
        return match.group(1)
    else:
        raise ValueError("No project name found")


import os
import glob
import shutil


def copy_files_by_pattern(source_dir, pattern, target_dir):
    """
    将源目录下符合通配符规则的文件复制到目标目录

    参数：
        source_dir (str): 需要复制文件的源目录路径
        pattern (str): 要匹配的文件名模式（支持通配符，如*.txt）
        target_dir (str): 文件复制的目标目录路径

    返回：
        list: 成功复制的文件路径列表

    异常：
        ValueError: 当源目录或目标目录不存在时抛出
    """

    # 检查源目录是否存在
    if not os.path.exists(source_dir):
        raise ValueError(f"源目录不存在：{source_dir}")

    # 检查目标目录是否存在，不存在则自动创建
    os.makedirs(target_dir, exist_ok=True)

    # 构建完整的文件搜索路径模式
    search_pattern = os.path.join(source_dir, pattern)

    # 获取匹配的文件列表（不包含子目录）
    matched_files = glob.glob(search_pattern)

    # 存储成功复制的文件路径
    copied_files = []

    # 遍历所有匹配到的文件
    for file_path in matched_files:
        try:
            # 确保是文件而不是目录
            if os.path.isfile(file_path):
                # 构造目标路径
                target_path = os.path.join(target_dir, os.path.basename(file_path))

                # 执行文件复制操作
                shutil.copy2(file_path, target_path)
                copied_files.append(target_path)
                print(f"成功复制：{file_path} -> {target_path}")
        except Exception as e:
            print(f"复制文件失败：{file_path}，错误信息：{str(e)}")

    return copied_files


# ----------------------------------输出结果处理--------------------------------------------
"""
UI代码转换系统 v2.5 
简介：
再看一眼就要爆炸
功能特性：
1. 模板驱动的代码结构生成 
2. 智能依赖关系解析（去除）
3. 多级代码校验机制 （去除）
4. 可扩展的钩子系统 
5. 分布式文件输出支持 
"""


class GenerateMode(Enum):
    """生成模式枚举"""
    OVERWRITE = 1
    MERGE = 2  # 新增融合模式，移除其他模式


@dataclass
class CppCodeTemplate:
    """
    代码模板配置容器
    Attributes:
        framework (str): cpp代码框架模板
        widgets_namespace (str): 组件定义命名空间模板
        init_namespace (str): 初始化逻辑命名空间模板
        function_template (str): 函数结构模板
        ui_interface (str): ui接口
    """
    framework: str = """\
{pre_define}
/*!INCLUDES_BEGIN!*/
{includes}

/*!INCLUDES_END!*/

// 组件定义 
{widgets_namespace}
{post_define}

/*!DECLARE_CODE_BEGIN!*/
// 全局变量定义

// 类声明

/*!DECLARE_CODE_END!*/

{pre_init}
{init_namespace}
{post_init}

{ui_interface}

/*!DEFINE_CODE_BEGIN!*/
// 类定义

/*!DEFINE_CODE_END!*/
"""
    widgets_namespace: str = """\
namespace {ns_name} 
{{
{define_blocks}
/*!WIDGETS_DEFINE_BEGIN!*/

/*!WIDGETS_DEFINE_END!*/
}}
using namespace {ns_name};"""

    init_namespace: str = """\
namespace {ns_name}
{{
    void {func_name}() 
    {{
{init_blocks}
    /*!INIT_CUSTOM_BEGIN!*/

    /*!INIT_CUSTOM_END!*/
    }}
    
    void events()
    {{
    /*!EVENTS_CUSTOM_BEGIN!*/
    
    /*!EVENTS_CUSTOM_END!*/
    }}
}}
"""

    ui_interface: str = """\
/*!UI_IMPLEMENT_BEGIN!*/
// 用户自定义接口实现区（安全编辑区） 
namespace {ns_name} {{


}}
/*!UI_IMPLEMENT_END!*/
"""

    # 初始化函数模板
    function_template: str = """\
    void {func_name}() 
    {{
{func_body}
    }}
    """


@dataclass
class HppCodeTemplate:
    """
    代码模板配置容器
    Attributes:
        framework (str): cpp代码框架模板
        widgets_namespace(str): 声明组件的模板
        ui_interface(str): ui接口
    Note:
        declare_namespace和ui_interface需要借助本类里的数据类型
    """
    framework: str = """\
{pre_define}
// Auto-generated UI Code, converted from GUI Guider
{includes}
 
{widgets_namespace}

// ------定义UI接口------
{ui_interface}

// ------加载资源------
{load_resources}  
"""

    widgets_namespace: str = """\
namespace {ns_name}
{{
{declare_blocks}
/*!WIDGETS_DECLARE_BEGIN!*/

/*!WIDGETS_DECLARE_END!*/
}}  
"""

    ui_interface: str = """\
/*!UI_INTERFACE_BEGIN!*/
// 用户自定义接口声明区（安全编辑区）
namespace {ns_name} {{


}}
/*!UI_INTERFACE_END!*/
"""


class HookSystem:
    """钩子管理系统"""

    HOOK_POINTS = (
        'pre_define',  # 定义块生成前
        'post_define',  # 定义块生成后
        'pre_init',  # 初始化块生成前
        'post_init',  # 初始化块生成后
        'post_generate'  # 全部生成完成后
    )

    def __init__(self):
        self.hooks = {point: [] for point in self.HOOK_POINTS}

    def add_hook(self, point: str, callback: Callable) -> None:
        """
        注册钩子函数
        Args:
            point: 钩子点名称，必须是HOOK_POINTS成员
            callback: 无参数的回调函数，返回字符串将插入代码
        """
        if point not in self.HOOK_POINTS:
            raise ValueError(f"无效钩子点: {point}")
        self.hooks[point].append(callback)

    def execute_hooks(self, point: str) -> str:
        """执行钩子并返回第一个非空的结果，如果没有结果则返回空字符串"""
        results = [hook() for hook in self.hooks.get(point, [])]
        # 过滤掉 None 和空字符串，并返回第一个非空结果，如果没有结果则返回空字符串
        filtered_results = list(filter(None, results))
        return filtered_results[0] if filtered_results else ''


def _build_load_resources(resources, is_custom=True) -> str:
    """构建资源加载代码"""
    resources_list = []

    # 处理字体资源
    font_resources = []
    for font in resources.get('font', []):
        # 应用custom前缀规则
        if is_custom:
            processed_font = font.replace("lv_font", "lv_customer_font", 1)  # 仅替换第一个匹配项
        else:
            processed_font = font
        font_resources.append(f"LV_FONT_DECLARE({processed_font})")

        # 添加字体区块
    if font_resources:
        resources_list.append("//  字体资源")
        resources_list.extend(font_resources)

        # 处理图片资源
    img_resources = [f"LV_IMG_DECLARE({img})" for img in resources.get('image', [])]

    # 添加图片区块
    if img_resources:
        resources_list.append("//  图片资源")
        resources_list.extend(img_resources)

        # 生成最终代码
    content = "\n".join(resources_list)
    return content


class TemplateGenerator:
    """模板驱动代码生成器"""

    def __init__(self):
        """
        Args:
        """
        self.cpp_template = CppCodeTemplate()
        self.hpp_template = HppCodeTemplate()
        self.placeholders = {
            'cpp_includes': '#include "ui.hpp"',
            'hpp_includes': '#include "GUI_Base.hpp"',
            'widgets_namespace': 'gui::widgets::main',
            'init_namespace': 'gui::init',
            'func_name': 'screen',
            'ui_interface': 'gui::interface'
        }
        self.hooks = HookSystem()

    def generate(
            self,
            define_blocks: List[str],
            init_blocks: List[str],
            output_path: str,
            is_font_custom: bool = True,
            mode: GenerateMode = GenerateMode.OVERWRITE
    ) -> bool:
        """
        执行代码生成流程
        Args:
            define_blocks: 组件定义代码块列表
            init_blocks: 初始化代码块列表
            output_path: 输出文件路径（最后不用带反斜杠）
            is_font_custom: 决定生成字体声明时，要不要使用自定义的字体类型。默认使用
            mode: 生成模式
        Returns:
            生成是否成功
        """
        # 阶段1：预处理
        # 校验以后可能有用，拓扑排序适合前面的代码处理
        # validated_defines = self._validate_blocks(define_blocks)
        # ordered_inits = self.resolver.get_ordered_blocks(init_blocks)

        # 阶段2：构建代码结构
        # hpp
        declare_widgets_content = self._build_declare_widgets_content(define_blocks)
        declare_ui_interface = self._build_declare_ui_interface([])
        load_resources = _build_load_resources(resources, is_font_custom)
        # cpp
        define_widgets_content = self._build_define_widgets_content(define_blocks)
        init_content = self._build_init_content(init_blocks, mode)
        define_ui_interface = self._build_define_ui_interface([])

        # 阶段3：生成最终代码
        # hpp
        final_hpp_code = self.hpp_template.framework.format(
            pre_define=self.hooks.execute_hooks('pre_define'),
            # 命名空间等
            includes=self.placeholders['hpp_includes'],
            widgets_namespace=declare_widgets_content,
            ui_interface=declare_ui_interface,
            load_resources=load_resources,
        )
        # cpp
        final_cpp_code = self.cpp_template.framework.format(
            # 一群钩子
            pre_define=self.hooks.execute_hooks('pre_define'),
            post_define=self.hooks.execute_hooks('post_define'),
            pre_init=self.hooks.execute_hooks('pre_init'),
            post_init=self.hooks.execute_hooks('post_init'),
            # 组件可以和初始化命名空间
            includes=self.placeholders['cpp_includes'],
            widgets_namespace=define_widgets_content,
            init_namespace=init_content,
            ui_interface=define_ui_interface,
        )

        # 阶段4：后处理
        print(self.hooks.execute_hooks('post_generate'))
        hpp_path = Path(output_path) / 'ui.hpp'
        cpp_path = Path(output_path) / 'ui.cpp'

        if mode == GenerateMode.OVERWRITE:
            hpp_path.write_text(final_hpp_code, encoding='utf-8')
            cpp_path.write_text(final_cpp_code, encoding='utf-8')
        elif mode == GenerateMode.MERGE:
            # 处理hpp文件
            if hpp_path.exists():
                existing_hpp = hpp_path.read_text(encoding='utf-8')
                merged_hpp = self._merge_code(existing_hpp, final_hpp_code, 'hpp')
                hpp_path.write_text(merged_hpp, encoding='utf-8')
            else:
                hpp_path.write_text(final_hpp_code, encoding='utf-8')
            # 处理cpp文件
            if cpp_path.exists():
                existing_cpp = cpp_path.read_text(encoding='utf-8')
                merged_cpp = self._merge_code(existing_cpp, final_cpp_code, 'cpp')
                cpp_path.write_text(merged_cpp, encoding='utf-8')
            else:
                cpp_path.write_text(final_cpp_code, encoding='utf-8')
        return True

    def _build_define_widgets_content(self, blocks: List[str]) -> str:
        """构建定义部分代码"""
        content = '\n'.join(blocks)
        return self.cpp_template.widgets_namespace.format(
            ns_name=self.placeholders['widgets_namespace'],
            define_blocks=content
        )

    def _build_declare_widgets_content(self, blocks: List[str]) -> str:
        """构建声明部分代码
        Note:
            输入的是定义代码块列表
        """
        declare_list = ['\textern ' + line for line in blocks]
        content = '\n'.join(declare_list)
        return self.hpp_template.widgets_namespace.format(
            ns_name=self.placeholders['widgets_namespace'],
            declare_blocks=content
        )

    def _build_declare_ui_interface(self, blocks: List[str]) -> str:
        """构建UI接口声明代码
        Note:
            输入的列表是外部声明的ui接口
        """
        ui_interface_declare = ['extern ' + line for line in blocks if line != '']
        content = '\n'.join(ui_interface_declare)
        return self.hpp_template.ui_interface.format(
            ns_name=self.placeholders['ui_interface'],
            declare_blocks=content
        )

    def _build_define_ui_interface(self, blocks: List[str]) -> str:
        """构建ui接口定义代码

        Note:
            ui定义代码，blocks的每个元素都是一段代码定义
        """
        content = '\n'.join(blocks)
        return self.cpp_template.ui_interface.format(
            ns_name=self.placeholders['ui_interface'],
            define_blocks=content
        )

    def _build_init_content(self, blocks: List[str], mode: GenerateMode) -> str:
        func_body = textwrap.indent('\n'.join(blocks), '')

        # 在初始化函数外围添加标记
        return self.cpp_template.init_namespace.format(
            ns_name=self.placeholders['init_namespace'],
            func_name=self.placeholders['func_name'],
            init_blocks=func_body
        )

    def _merge_code(self, existing: str, new: str, file_type: str) -> str:
        """智能合并策略"""
        # 定义保留规则(目前只有KEEP_WHOLE)可以使用
        preserve_rules = {
            'hpp': [
                (r'/\*!UI_INTERFACE_BEGIN!.*?\*!UI_INTERFACE_END!', 'KEEP_WHOLE'),  # 全保留
                (r'/\*!WIDGETS_DECLARE_BEGIN!.*?\*!WIDGETS_DECLARE_END!', 'KEEP_WHOLE')  # 合并内容
            ],
            'cpp': [
                (r'/\*!INCLUDES_BEGIN!.*?\*!INCLUDES_END!', 'KEEP_WHOLE'),
                (r'/\*!WIDGETS_DEFINE_BEGIN!.*?\*!WIDGETS_DEFINE_END!', 'KEEP_WHOLE'),
                (r'/\*!DECLARE_CODE_BEGIN!.*?\*!DECLARE_CODE_END!', 'KEEP_WHOLE'),
                (r'/\*!INIT_CUSTOM_BEGIN!.*?\*!INIT_CUSTOM_END!', 'KEEP_WHOLE'),
                (r'/\*!EVENTS_CUSTOM_BEGIN!.*?\*!EVENTS_CUSTOM_END!', 'KEEP_WHOLE'),
                (r'/\*!UI_IMPLEMENT_BEGIN!.*?\*!UI_IMPLEMENT_END!', 'KEEP_WHOLE'),
                (r'/\*!DEFINE_CODE_BEGIN!.*?\*!DEFINE_CODE_END!', 'KEEP_WHOLE'),

            ]
        }

        for pattern, mode in preserve_rules.get(file_type, []):
            # 查找现有代码中的匹配区域
            old_match = re.search(pattern, existing, re.DOTALL)
            if not old_match:
                continue

            # 根据模式处理
            if mode == 'KEEP_WHOLE':
                # 用旧区域完全替换新区域
                new = re.sub(pattern, old_match.group(0), new, count=1, flags=re.DOTALL)
            elif mode == 'MERGE_CONTENT':
                # 提取旧内容并插入新框架
                old_content = self._extract_inner_content(old_match.group(0))
                new = self._replace_inner_content(new, pattern, old_content)
            elif mode == 'KEEP_CONTENT':
                # 保留旧内容但保持新框架
                old_content = self._extract_inner_content(old_match.group(0))
                new = self._replace_inner_content(new, pattern, old_content, keep_markers=True)

        return new

    def _extract_inner_content(self, text: str) -> str:
        """提取两个标记之间的内容（不含标记本身）"""
        match = re.search(r'(?<=BEGIN!)[\s\S]*?(?=END!)', text, re.DOTALL)
        return match.group(0).strip() if match else ''

    def _replace_inner_content(self, new_code: str, pattern: str, content: str, keep_markers=False) -> str:
        """替换区域内部内容"""

        def replacer(match):
            if keep_markers:
                return f"{match.group(1)}{content}{match.group(3)}"
            return f"{match.group(1)}{content}{match.group(3)}"

        return re.sub(
            r'(/\*!.*?_BEGIN!)(.*?)(\*!.*?_END!*/)',
            replacer,
            new_code,
            flags=re.DOTALL,
            count=1
        )


# -------------------------------------主函数--------------------------------------------
def main():
    # 【功能】：自定义字体
    is_font_custom = True
    # 【定位屏幕初始化代码】：定位 setup_scr_* 函数,并获取工程名和函数体
    c_content = find_c_functions(search_path=r"E:\Program\Embedded\MCU\GUI\GUI\generated",
                                 func_name="setup_scr_*",
                                 file_name="setup_scr_*.c")
    # 获取第一个满足条件的函数
    setup_scr_function = c_content[0]
    # 获取函数体和屏幕名称
    screen_name = extract_screen_name(setup_scr_function[0])
    func_body = setup_scr_function[2]
    # 添加屏幕组件关系
    add_widget_relations(screen_name)

    # 【切割代码行】：从setup_scr_*函数体代码提取出代码行
    code_lines = extract_c_code_lines(func_body)

    # 【处理代码行】：从每行代码里提取到组件初始化信息和组件关系
    process_code_lines(code_lines)

    # 【解析信息】：把组件信息解析为对应的代码，并输出
    widgets_init_code, widgets_define_code = iterate_widgets(screen_name, is_font_custom=is_font_custom)

    # 【输出代码文件】：把组件相关代码信息，按照特定格式写入文件中
    generator = TemplateGenerator()

    # 添加版本信息钩子
    generator.hooks.add_hook('pre_define', lambda: f"// Generated by Fairy on {datetime.now():%Y-%m-%d  %H:%M}\n")

    # 执行生成（与工作目录有关）
    generator.generate(
        define_blocks=widgets_define_code,
        init_blocks=widgets_init_code,
        output_path="../../Projects/driversDevelop/ui",
        is_font_custom=is_font_custom,
        mode=GenerateMode.OVERWRITE
    )

    print("代码生成成功！输出文件：ui.cpp")

    # 复制所有字体
    copy_files_by_pattern(source_dir=r"E:\Program\Embedded\MCU\GUI\GUI\generated\guider_fonts",
                          target_dir="../../Projects/driversDevelop/ui",
                          pattern="*.c")
    # 复制所有图片
    copy_files_by_pattern(source_dir=r"E:\Program\Embedded\MCU\GUI\GUI\generated\images",
                          target_dir="../../Projects/driversDevelop/ui",
                          pattern="*.c")


if __name__ == "__main__":
    # 在执行其他命令之前设置代码页为UTF-8
    os.system('chcp 65001')
    main()
