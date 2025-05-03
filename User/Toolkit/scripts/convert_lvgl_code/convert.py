import json
import extract_info
import parser
import generate_code
import copy_files
import yaml

if __name__ == "__main__":
    # 输入要转换的文件路径
    root_path = r"D:/Program/GUI-GUIDER/GUI/generated"
    # 输入目标路径
    target_path =r"../../../../Projects/RLC/ui"

    # ====== 提取代码信息 ======
    results = extract_info.extract(root_path=root_path,file_pattern="setup_scr_*.c")
    # ====== 解析代码信息 ======
    results = parser.parser(results)
    # ====== 生成代码 ======
    # 默认不覆写，输入输出路径
    generate_code.generate_code(file_path=target_path,data=results, overwrite=True)

    # ====== 复制所有字体 ======
    copy_files.copy_files_by_pattern(source_dir=f"{root_path}/guider_customer_fonts",target_dir=target_path,pattern="*.c")
    # ====== 复制所有图片 ======
    copy_files.copy_files_by_pattern(source_dir=f"{root_path}/images",target_dir=target_path,pattern="*.c")