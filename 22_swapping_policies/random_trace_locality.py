import random

locality_p = 0.75
last_address = -1

def gen_address():
    global last_address # warning: don't do this lol
    if last_address != -1:
        if random.random() >= locality_p:
            last_address = random.randint(0, 9)
    else:
        last_address = random.randint(0, 9)

    return last_address

trace = [gen_address() for i in range(10)]

print(trace)