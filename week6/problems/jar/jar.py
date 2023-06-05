class Jar:
    def __init__(self, capacity=12):
        self.capacity = capacity
        self.size = 0

    def __str__(self):
        return "🍪" * self.size

    def deposit(self, n):
        self.size += n

    def withdraw(self, n):
        self.size -= n

    @property
    def capacity(self):
        return self._capacity

    @capacity.setter
    def capacity(self, value):
        if value < 0:
            raise ValueError("Not possible to assign negative capacity.")
        self._capacity = value

    @property
    def size(self):
        return self._size

    @size.setter
    def size(self, value):
        self._size = value
        if self._size > self.capacity:
            self._size -= 1
            raise ValueError("Could not deposit. Reached the capacity limit.")
        elif self._size < 0:
            self._size = 0
            raise ValueError("Could not withdraw. No more cookies left. :(")

def main():
    jar = Jar()

    print(str(jar.capacity))
    print(str(jar))

    jar.deposit(2)
    print(str(jar))

    jar.withdraw(1)
    print(str(jar))

main()