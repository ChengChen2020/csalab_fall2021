with open('trace.out', 'r') as test, open('trace_grading', 'r') as reference:
    for line1 in test:
        for line2 in reference:
            if line1 != line2:
                print(line1)
                print(line2)
            break
