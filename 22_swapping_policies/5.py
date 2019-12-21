addresses = []

with open("ls.txt", "r+") as f:
    for line in f:
        if not line.startswith("="):
            parts = line.strip().split()
            address_str = parts[1].split(",")[0]
            address = int(address_str, 16) >> 12
            print(address)

# print(",".join(addresses))