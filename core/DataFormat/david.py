class foo:
    
    def __init__(self):
        self.data=[]
        self.index=0

    def append(self,obj):
        self.data.append(obj)

    def __eq__(self,obj):
        return (obj == self.data)

    def __neq__(self,obj):
        return (not self == obj)

    def __iter__(self):
        return self

    def next(self):
        if self.index == len(self.data):
            raise StopIteration
        self.index += 1
        return self.data[self.index-1]

    def __str__(self):
        return str(self.data)

if __name__=='__main__':
    k=foo()

    k.append(1)

    for x in k:
        print x

    mydata = [1]
    print 'Is',k,'same as',mydata,'?'
    print k == mydata
    print 'Or maybe not?'
    print not k==mydata
    print 'Describe yourself!'
    print k
