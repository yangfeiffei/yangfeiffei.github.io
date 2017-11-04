import argparse
parser = argparse.ArgumentParser(description="prog help...",   # 描述
                                 prog="prog",                  # 这个命令的名称
                                 usage="hhhh...a..."           # 用法
                                 )
# 增加必选参数
# parser.add_argument("echo",                                    # 必选参数名称
#                     help="echo the string you use here."       # 帮助信息
#                     )
# 增加可选参数
parser.add_argument("-s",                                      # 可选参数
                    "--square",                                # 可选参数，完整
                    help="echo the square of your given number.",
                    type=int                                   # 参数的类型
                    )
parser.add_argument("-v","--verb", help="increase output verbosity.",
                    action="store_true"                        # 后面不再跟参数值
                    )

arg = parser.parse_args()

# 开始处理输入的参数：
if arg.square:
    print(arg.square**2)

if arg.verb:
    print("verb turned on.")
