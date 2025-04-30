import glob
import os
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