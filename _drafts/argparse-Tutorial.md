


argparse是python标准库，基于optparse。

# 1.基本

写一个最简单的例子，prog.py

```python
import argparse
parser = argparse.ArgumentParser()
parser.parse_args()
```

可以发现很方便的类似bash命令的帮助信息。
```python
localhost:scripts$ python prog.py --help
usage: prog.py [-h]

optional arguments:
  -h, --help  show this help message and exit
```


# 2.必选参数


```python
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("echo")
arg = parser.parse_args()
print(arg.echo)
```



# 3.可选参数
```python
import argparse
parser = argparse.ArgumentParser()
# parser.add_argument("echo", help="echo the string you use here.")
parser.add_argument("--square", help="echo the square of your given number.", type=int)
parser.add_argument("--verb", help="increase output verbosity.", action="store_true")
arg = parser.parse_args()
if arg.square:
    print(arg.square**2)
if arg.verb:
    print("verb turned on.")
```












# 参考

- https://docs.python.org/3/howto/argparse.html
