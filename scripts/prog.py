import argparse
parser = argparse.ArgumentParser(description="prog help...",   # 描述
                                 prog="prog",                  # 这个命令的名称
                                 usage="hhhh...a..."           # 用法
                                 )
增加必选参数
parser.add_argument("addnode",                                    # 必选参数名称
                    help="add a node"       # 帮助信息
                    )

parser.add_argument("-h",
                    "--hostname",
                    help="node hostname",
                    )
parser.add_argument("-i",
										"--ipaddr", 
										help="node ip address",
                    )
parser.add_argument("-m",
										"--mac", 
										help="node mac address",
                    )
                    

arg = parser.parse_args()

# 开始处理输入的参数：
if arg.square:
    print(arg.square**2)

if arg.verb:
    print("verb turned on.")
