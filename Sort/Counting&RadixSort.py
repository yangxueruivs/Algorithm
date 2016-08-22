def countingSort(a, k, d):
    out, temp = [0 for i in range(len(a))], [0 for i in range(k)]
    for i in range(len(a)):
        print a[i]/d%10
        temp[a[i]/d%10] += 1
    for j in range(1, k):
        temp[j] += temp[j-1]
    for l in range(len(a)-1, -1, -1):
        out[temp[a[l]/d%10]-1] = a[l]
        temp[a[l]/d%10] = temp[a[l]/d%10] - 1
    return out
    
def radixSort(a, d):
    for i in range(d):
        a = countingSort(a, 10, 10**i)
    return a
        