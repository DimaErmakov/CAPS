from basicsearch import *


class Pitcher_State:
    def __init__(self, q3, q4):
        self.q3 = q3
        self.q4 = q4
        self.operator_list = [
            "fillq3",
            "fillq4",
            "drainq3",
            "drainq4",
            "pourq3toq4",
            "pourq4toq3",
        ]

    def __str__(self):
        state_string = "(" + str(self.q3) + "," + str(self.q4) + ")"
        return state_string

    def illegal(self):
        if self.q3 < 0 or self.q4 < 0 or self.q3 > 3 or self.q4 > 4:
            return True
        else:
            return False

    def equals(self, state):
        if self.q3 == state.q3 and self.q4 == state.q4:
            return True
        else:
            return False

    def operator_names(self):
        return self.operator_list

    def fillq3(self):
        return Pitcher_State(3, self.q4)

    def fillq4(self):
        return Pitcher_State(self.q3, 4)

    def drainq3(self):
        return Pitcher_State(0, self.q4)

    def drainq4(self):
        return Pitcher_State(self.q3, 0)

    def pourq3toq4(self):
        capacity = 4 - self.q4
        if self.q3 > capacity:
            return Pitcher_State(self.q3 - capacity, 4)
        else:
            return Pitcher_State(0, self.q4 + self.q3)

    def pourq4toq3(self):
        capacity = 3 - self.q3
        if self.q4 > capacity:
            return Pitcher_State(3, self.q4 - capacity)
        else:
            return Pitcher_State(self.q3 + self.q4, 0)

    def apply_operators(self):
        return [
            self.fillq3(),
            self.fillq4(),
            self.drainq3(),
            self.drainq4(),
            self.pourq3toq4(),
            self.pourq4toq3(),
        ]


Search(Pitcher_State(0, 0), Pitcher_State(0, 2))
