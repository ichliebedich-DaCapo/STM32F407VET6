# 现在里面存储了各式各样的信息
#   变量类型：变量名、值和可能的变量类型，用以当成键值，匹配到【无变量类型且有lv_obj类似的函数调用】，那么就作为变量类型确定，收获【变量名】和组件类型且有
#   函数调用类型：第一个参数是必然是组件类型，以此为键值与前面的匹配到一起
#   前面两个都没匹配到的，打印信息来警告
# 组织信息：
#   根据键值把不同的函数调用组合放到字典里，{var,funcs},funcs:{func_name,['arg' 'arg' '']} 此时是info字典
#
#   变量定义转换：
#       去除ui->前缀，添加组件类型前缀，并根据[值]来确定组件类型，压入一个列表里，作为变量定义。如果找不到就用原lvgl函数
#   链式调用转换：
#       根据函数名来确定转换后的函数名，压入一个列表里，作为函数调用
#   组合信息：
#       定义一个字典，保存var（去除ui->）、变量定义和函数定义
import json
import os
import re
from collections import defaultdict

import yaml
from pathlib import Path
def is_field_valid(config_dict, key_path):
    """
    检查字段存在且值不为空
    :param config_dict: 配置字典（如整个 config 或嵌套子字典）
    :param key_path: 要检查的字段名（单层）
    :return: bool (True=存在且非空)
    """
    return key_path in config_dict and config_dict[key_path] is not None

def load_config():
    with open('config.yml', encoding='utf-8') as f:  # 添加编码参数
        config = yaml.safe_load(f) or {}
    return config
cfg = load_config()

def validate_params(params, defaults, arg_index):
    # 复制一份 params 避免修改原列表
    params = list(params)
    j = len(defaults) - 1  # 从右往左遍历 defaults
    all_matched = True     # 标记是否全部匹配成功

    while j >= 0 and len(params) > arg_index + 1:
        if params[-1] == defaults[j]:
            params.pop()  # 移除匹配项
            j -= 1
        else:
            all_matched = False  # 遇到不匹配项，终止
            break

    return params, all_matched

def parser(info):
    # 检查配置文件的完整性
    if not is_field_valid(cfg, 'mappings'):
        ValueError("配置文件缺少 'mappings' 字段")
    cfg_mappings = cfg['mappings']
    cfg_skip_rules = cfg['skip_rules']

    if not is_field_valid(cfg_mappings, 'variable_types') or not is_field_valid(cfg_mappings, 'function_calls'):
        ValueError("配置文件缺少 'variables' 或 'calls' 字段")
    cfg_variables = cfg_mappings['variable_types']
    cfg_calls = cfg_mappings['function_calls']
    cfg_style_calls = cfg_mappings['style_calls']


    combin_info = {}
    # ======================= 组合信息 ==================
    """
        {
        'widget':{
                'variables':[xxx]
                'calls':[xxx]
            },
            ……
        }
    """
    for file_path, data in info.items():
        # 以变量名为键
        combin_info[file_path] = {}# 添加文件信息
        for var in data['variables']:
            var_name = var['name'].replace('ui->','')
            # 去除变量名后的字典
            clean_variable = {k: v for k, v in var.items() if k != 'name'}
            # 添加变量信息,
            if is_field_valid(combin_info[file_path],  var_name):
                combin_info[file_path][var_name]['variables'].append(clean_variable)
            else:
                combin_info[file_path][var_name] = {'variables': [clean_variable], 'calls': []}

            # 添加函数调用信息
            for call in data['calls']:
                if var['name'] == call['params'][0]:
                    combin_info[file_path][var_name]['calls'].append(call)


    # ======================= 词法解析 ==================
    process_results ={}
    for file_path,data in combin_info.items():
        print(f"\r\n处理文件: {os.path.basename(file_path)}")
        """
        calls = {
                    'widget':{
                        chain:['call' 'call' ''],
                        lvgl: ['call' 'call' '']
                    },
                    'widget':{
                        chain:['call' 'call' ''],
                        lvgl: ['call' 'call' '']
                    }
                }
        """
        variables = []
        calls = defaultdict(lambda: {
            'chain': [],
            'lvgl': []
        })
        resource =  {
            'fonts': [],
            'img': []
        }
        parent_name = '' # 父组件名称

        # ======== 对每个组件进行处理 ========
        for widget_name,widget_info in data.items():
            # ================== 组件定义 ==================
            for var in widget_info['variables']:
                # 对于每个变量定义
                value = var['value']
                var_type = var['var_type'] # 不是给变量定义用的
                func_name  = '' # 组件创建的函数名

                # === 获取组件创建的函数名和父组件名 ===
                pattern = r'(\w+)\(([^)]+)'  # 我踏马找疯了才找到是你的问题
                match = re.search(pattern, value)
                if match:
                    func_name = match.group(1)
                    parent_name = match.group(2).replace('ui->', '').replace("NULL", "nullptr")
                    # 获得合成名的前缀
                    pattern  = r'lv_(\w+)_create'
                    match = re.search(pattern, func_name)
                    if match:
                        prefix = match.group(1)
                        # 加前缀是个吃力不讨好的事情，已经剔除
                    else:
                        ValueError(f"无法解析变量定义: {value}")
                else:
                    # 变量赋值出现了不该出现的，比如定义样式，需要打个补丁来处理
                    ValueError(f"无法解析变量定义: {value}")

                # 确定变量的组件类型
                if is_field_valid(cfg_variables, func_name):
                    widget_type = cfg_variables[func_name]
                else:
                    widget_type = 'Component'

                # 添加变量定义

                variables.append(f'inline {widget_type} {widget_name};')

                # 添加变量初始化语句
                calls[widget_name]['chain'].append(f'{widget_name}.init({parent_name})')


            # # ================== 函数调用 ==================
            widget_calls = calls[widget_name]
            for call in widget_info['calls']:
                method_name = ''
                # ============= 资源获取 ============
                """
                resource:{
                        'fonts':[]
                        'img':[]
                    }
                """
                if 'text_font' in call['name']:
                    font = call['params'][1].replace("&", "")
                    if font not in resource['fonts']:  # 检查是否已存在
                        resource['fonts'].append(font)

                if 'image_set_src' in call['name']:
                    img = call['params'][1].replace("&", "")
                    if img not in resource['img']:  # 检查是否已存在
                        resource['img'].append(img)

                # ============= 跳过规则 ============
                if call['name'] in cfg_skip_rules['calls']:
                    continue

                # ============= 样式函数映射 ============
                if 'lv_obj_set_style_' in call['name']:
                    match = re.match(r"lv_obj_set_style_([a-zA-Z_]+)", call['name'])
                    if match:
                        method_name = match.group(1)  # 捕获函数名
                    if is_field_valid(cfg_style_calls, method_name):
                        # 默认形参
                        remaining_params = call['params'][1:]
                        if "LV_PART_MAIN|LV_STATE_DEFAULT" in call['params'][2]:
                            remaining_params.pop(1)
                            if cfg_style_calls[method_name] in call['params'][1]:
                                remaining_params.pop(0)
                                print(f'[ignore] {widget_name}  {method_name}')
                                continue

                        # 添加样式设置语句
                        params = ','.join(remaining_params)
                        widget_calls['chain'].append(f'.{method_name}({params})')
                        continue

                # ============= 普通函数映射 ============
                if is_field_valid(cfg_calls, call['name']):
                    cfg_call_name = cfg_calls[call['name']]
                    if not is_field_valid(cfg_call_name, 'name'):
                        # 配置文件缺少 'name' 字段
                        ValueError(f"Please 补充 'name' 字段到 'calls' 配置文件中")

                    method_name = cfg_call_name['name'] # 默认函数名
                    rename_flag = False
                    arg_index = 0 # 默认为0，表示没有参数名称映射规则
                    # ========= 参数名称映射规则 ==========
                    if is_field_valid(cfg_call_name,'naming_rule'):
                        cfg_naming_rules = cfg_call_name['naming_rule']
                        # 判断里面有没有规则
                        if is_field_valid(cfg_naming_rules, 'source_arg') and is_field_valid(cfg_naming_rules, 'pattern'):
                            # 提取参数映射名称
                            arg_index = cfg_naming_rules['source_arg']
                            cfg_calls_name_pattern = cfg_naming_rules['pattern']
                            if is_field_valid(cfg_calls_name_pattern,call['params'][arg_index]):
                                # 找到就更替名称，同时留下标志，让后面缺省时可以知道
                                rename_flag = True
                                method_name = cfg_calls_name_pattern[call['params'][arg_index]]

                    # ================== 函数形参缺省规则 ================
                    remaining_params = [x for x in call['params']] # 剩余形参表里剩下的参数
                    if is_field_valid(cfg_call_name,'defaults'):
                        defaults = cfg_call_name['defaults']
                        remaining_params,optional = validate_params(call['params'],defaults,arg_index)

                    # 是否去除索引处
                    if rename_flag:
                        remaining_params.pop(arg_index) #  去掉索引处
                    if remaining_params:
                        remaining_params.pop(0) # 去掉第一个参数

                    # 判断为空是否可以省略
                    if not remaining_params:
                        if is_field_valid(cfg_call_name, 'optional'):
                            if cfg_call_name['optional']:
                                print(f'[ignore] {widget_name}  {method_name}')
                                continue
                    params = ','.join(remaining_params)
                    widget_calls['chain'].append(f'.{method_name}({params})')

                else:
                    # ============ 怎么来的就怎么回去，使用原装lvgl函数 ============
                    method_name = call['name']
                    temp_params = call['params']
                    temp_params[0] = widget_name # 替换组件名
                    params = ','.join(temp_params)
                    widget_calls['lvgl'].append(f'{method_name}({params});')

        # ========== 组合信息 ==========
        process_results[file_path] ={
            'variables':variables,# 变量定义部分
            'calls':calls,# 函数调用部分
            'resource':resource,# 资源声明部分
        }
    return process_results

# 使用示例
if __name__ == "__main__":
    results = {
        "/path/to/file1.c":{
            "variables": [
                {
                    'name': 'ui->screen',
                    'value': 'lv_btn_create(ui->screen)',
                    'var_type': 'lv_obj_t*'
                }
            ],
            "calls": [
                {
                    'name': 'lv_obj_set_pos',
                    'params': ['ui->press', '100','120']
                },
                {
                    'name': 'lv_obj_set_size',
                    'params': ['ui->press', '100','20']
                },
                {
                    'name': 'lv_obj_set_text_size',
                    'params': ['ui->press', '100','20']
                },
                {
                    'name': 'lv_obj_set_text_font',
                    'params': ['ui->press', '&lv_font_SourceHanSerifSC_Regular_13','LV_PART_MAIN|LV_STATE_DEFAULT']
                },
                {
                    'name': 'lv_image_set_src',
                    'params': ['ui->press', '&_dianzisheji_RGB565A8_83x55']
                },
                {
                    'name': 'lv_obj_set_style_bg_opa',
                    'params':['ui->press', 'lv_color_hex(0xffffff)','LV_PART_MAIN|LV_STATE_DEFAULT']
                }
            ],
        },
    }
    results = parser(results)
    print(json.dumps(results, indent=2))

