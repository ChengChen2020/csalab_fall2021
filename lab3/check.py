with open('trace.txt.out', 'r') as test, open('uni_test/testcases/3/trace.ans', 'r') as reference:
    cnt = 0
    for line1 in test:
        for line2 in reference:
            if line1 != line2:
                # print(line1)
                # print(line2)
                cnt += 1
            break
print(cnt)