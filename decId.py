
def decreaseId():
    f = open('id.txt', 'r')
    id = f.read()
    id = int(id) - 1
    f.close()

    f = open('id.txt', 'w')
    f.write(str(id))
    f.close()
    return id


    
