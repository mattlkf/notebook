with open('small4_raw.txt') as f:
    vals = []
    for line in f:
      for x in line.split():
        vals.append(float(x))
      # vals.append([float(x) for x in line.split()])

    highest = max(vals)
    lowest = min(vals)


    print highest
    print lowest
    
