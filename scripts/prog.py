import argparse
parser = argparse.ArgumentParser(description="prog help...",prog="prog",
                                 usage="hhhh...a...")
# parser.add_argument("echo", help="echo the string you use here.")
parser.add_argument("-s","--square", help="echo the square of your given number.", type=int)
parser.add_argument("-v","--verb", help="increase output verbosity.", action="store_true")
arg = parser.parse_args()
if arg.square:
    print(arg.square**2)

if arg.verb:
    print("verb turned on.")
