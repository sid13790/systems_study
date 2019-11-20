import sys, re, functools

fname = sys.argv[1]

job_regex_str = r"^  Job (\d+) \( length = (\d+), tickets = (\d+) \)$"
job_regex_cmp = re.compile(job_regex_str)

random_regex_str = r"^Random (\d+)$"
random_regex_cmp = re.compile(random_regex_str)

JOB_NUM_IDX = 0
JOB_LEN_IDX = 1
JOB_TIX_IDX = 2

job_values = []
random_values = []

with open(fname, "r+") as f:
    for line in f:
        job_match = job_regex_cmp.match(line)
        if job_match is not None:
            job_values.append(
                list(map(
                    lambda x: int(x),
                    job_match.groups()
                ))
            )

        random_match = random_regex_cmp.match(line)
        if random_match is not None:
            random_values.append(int(random_match.group(1)))

print(job_values)
print(random_values)

total_ticket_value = functools.reduce(lambda sum, job: sum + job[JOB_TIX_IDX], job_values, 0)
job_run_list = []

for val in random_values:
    cur_ticket = val % total_ticket_value
    print(cur_ticket)
    counter = 0
    cur_job = 0
    for job in job_values:
        if (job[JOB_LEN_IDX] != 0):
            counter += job[JOB_TIX_IDX]
            if (counter > cur_ticket):
                job[JOB_LEN_IDX] -= 1
                total_ticket_value = functools.reduce(lambda sum, job: sum + (job[JOB_TIX_IDX] if job[JOB_LEN_IDX] != 0 else 0), job_values, 0)
                cur_job = job[JOB_NUM_IDX]
                print(job_values)
                break
    
    print(f"Run {cur_job}")
    job_run_list.append(cur_job)

print(job_run_list)