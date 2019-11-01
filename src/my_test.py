import example as e

class Molly(e.KwargsNamedClass):
    def __init__(self):
        super().__init__(name="Molly", **{})

def add(a, b):
    return a + b

def get_list():
    return [i for i in range(0,10)]

def get_dict():
    return {str(i): i for i in range(0,10)}

def get_my_struct():
    data = {"name": "fooooo", "prop": 2}
    d = e.DictNamedClass(data=data)
    print(d)
    p = e.KwargsNamedClass(**data)
    p.name = "POPEL"
    print(p)
    res = Molly()
    print(res)
    return res
