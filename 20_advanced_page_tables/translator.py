import subprocess
import re

SEEDS = [0] #[0, 1, 2]
NUM_PHYSICAL_PAGES = 128
NUM_VIRTUAL_PAGES = 1024
PAGE_SIZE = 32

page_re = re.compile(r"^page\s+(\d+):(.+)$")
pdbr_re = re.compile(r"^PDBR: (\d+).+$")
vadd_re = re.compile(r"^Virtual Address ([0-9a-fA-F]+):.+$")

# gets a segment of an integer given a mask and a shift
def get_segment(i, mask, shift):
    return (i & (mask << shift)) >> shift

# translate a virtual address into a physical address given:
# - the address space
# - page directory base register (pdbr)
# virtual addresses are:
# - 5 bits of offset
# - 10 bits for VPN
#   - 5 bits for offset (offset into the PT frame/page that we get from the PDE)
#   - 5 bits for PDE (there are 32 pages worth of the PT)
# - 1 bit for validity
def translate_address(address_space, pdbr, virtual_address):
    print(f"Virtual Address {virtual_address:04x}:")

    # grab the page directory address
    page_directory_address = pdbr * PAGE_SIZE

    # grab the page directory entry
    PAGE_DIRECTORY_OFFSET_MASK = 0b11111
    PAGE_DIRECTORY_OFFSET_SHIFT = 10

    page_directory_offset = get_segment(
        virtual_address,
        PAGE_DIRECTORY_OFFSET_MASK, 
        PAGE_DIRECTORY_OFFSET_SHIFT
    )

    page_directory_entry = address_space[page_directory_address + page_directory_offset]

    # check validity of PDE
    VALID_PDE_MASK = 0b1
    VALID_PDE_SHIFT = 7

    valid_pde = get_segment(
        page_directory_entry, 
        VALID_PDE_MASK, 
        VALID_PDE_SHIFT
    )

    # get page table frame number from PDE
    PAGE_TABLE_FRAME_MASK = 0b1111111
    PAGE_TABLE_FRAME_SHIFT = 0

    page_table_frame = get_segment(
        page_directory_entry,
        PAGE_TABLE_FRAME_MASK, 
        PAGE_TABLE_FRAME_SHIFT
    )

    print(f"  --> pde index:{page_directory_offset:#02x} [decimal {page_directory_offset}] pde contents:{page_directory_entry:#02x} (valid {valid_pde}, pfn {page_table_frame:#02x} [decimal {page_table_frame}])")


    if valid_pde != 1:
        print("      --> Fault (page directory entry not valid)")
        return None

    # get page table entry
    PAGE_TABLE_FRAME_OFFSET_MASK = 0b11111
    PAGE_TABLE_FRAME_OFFSET_SHIFT = 5

    page_table_frame_offset = get_segment(
        virtual_address,
        PAGE_TABLE_FRAME_OFFSET_MASK,
        PAGE_TABLE_FRAME_OFFSET_SHIFT
    )

    page_table_entry = address_space[page_table_frame * PAGE_SIZE + page_table_frame_offset]

    # check validity of PTE
    VALID_PTE_MASK = 0b1
    VALID_PTE_SHIFT = 7

    valid_pte = get_segment(
        page_table_entry, 
        VALID_PTE_MASK, 
        VALID_PTE_SHIFT
    )

    # get physical frame from page table entry
    PHYSICAL_FRAME_MASK = 0b1111111
    PHYSICAL_FRAME_SHIFT = 0

    physical_frame = get_segment(
        page_table_entry,
        PHYSICAL_FRAME_MASK, 
        PHYSICAL_FRAME_SHIFT
    )

    print(f"    --> pte index:{page_table_frame_offset:#02x} [decimal {page_table_frame_offset}] pte contents:{page_table_entry:#02x} (valid {valid_pte}, pfn {physical_frame:#02x} [decimal {physical_frame}])")

    if valid_pte != 1:
        print("      --> Fault (page table entry not valid)")
        return None

    # get exact address with physical frame and offset
    PHYSICAL_FRAME_OFFSET_MASK = 0b11111
    PHYSICAL_FRAME_OFFSET_SHIFT = 0

    physical_frame_offset = get_segment(
        virtual_address,
        PHYSICAL_FRAME_OFFSET_MASK,
        PHYSICAL_FRAME_OFFSET_SHIFT
    )

    physical_address = physical_frame * PAGE_SIZE + physical_frame_offset

    print(f"      --> Translates to Physical Address {physical_address:#04x} --> Value: {address_space[physical_address]:02x}")
    


# solve given the stdout of the simulator
def solve(output):

    # print(output)

    # grab the lines
    lines = output.split("\n")

    # array that holds all the page strings from the output
    page_matches = []

    # the virtual addresses to solve for
    virtual_addresses = []

    # the whole address space we'll be solving over
    address_space = []

    # value of PDBR
    pdbr = -1

    # parse through all the lines and collect the pages + PDBR
    for line in lines:
        page_match = page_re.match(line)
        if page_match is not None:
            page_matches.append(page_match)
            continue

        pdbr_match = pdbr_re.match(line)
        if pdbr_match is not None:
            pdbr = int(pdbr_match.groups()[0])
            continue

        vadd_match = vadd_re.match(line)
        if vadd_match is not None:
            virtual_address = int(vadd_match.groups()[0], 16)
            virtual_addresses.append(virtual_address)

    # convert the page matches to a proper address space
    for page_match in page_matches:
        idx, data = page_match.groups()

        addresses = list(map(lambda x: int(x, 16), data.strip().split(' ')))
        address_space.extend(addresses)

    print(virtual_addresses)
    print(pdbr)

    for virtual_address in virtual_addresses:
        translate_address(address_space, pdbr, virtual_address)


for seed in SEEDS:
    p = subprocess.Popen(
        ["./paging-multilevel-translate.py", "-s", str(seed)],
        stdout = subprocess.PIPE,
        stderr = subprocess.PIPE
    )

    stdout, _ = p.communicate()
    output = stdout.decode("utf-8")

    solve(output)
