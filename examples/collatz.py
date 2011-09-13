# collatz.py
#
# This will print out a tree representing
#+ Collatz's conjecture in reverse
# See collatz.dcm for roughly equivalent code

LIMIT = 100

class Collatz:
    
    def __init__(self, val):
        global LIMIT
        self.val = val
        if val < LIMIT:
            self.trunk = Collatz(val * 2)
            if val > 4 and val % 6 == 4:
                self.branch = Collatz((val - 1) / 3)
    
    def print_inner(self, prefix):
        print prefix + str(self.val)
        if hasattr(self, "branch"):
            new_prefix = prefix + ' ' * len(str(self.val)) + "| "
            self.branch.print_inner(new_prefix)
            del new_prefix
        if hasattr(self, "trunk"):
            self.trunk.print_inner(prefix)
    
    def prints(self):
            self.print_inner("| ");

Collatz(1).prints()
