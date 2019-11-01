from example import MyStruct

class Molly(MyStruct):
    def __init__(self):
        super().__init__(name="Molly")

def add(a, b):
    return a + b

def get_list():
    return [i for i in range(0,10)]

def get_dict():
    return {str(i): i for i in range(0,10)}

def get_my_struct():
    empty_struct = MyStruct()
    print(empty_struct.getName())
    res = Molly()
    print(res.getName())
    return res
