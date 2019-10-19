def add(a, b):
    return a + b

def get_list():
    return [i for i in range(0,10)]

def get_dict():
    return {str(i): i for i in range(0,10)}

def get_my_struct():
    res = MyStruct("Molly")
    print(res.getName())
    return res
