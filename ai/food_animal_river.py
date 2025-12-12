from basicsearch import *


def colorize(side):
    if side == "left":
        return f"\033[91m{side}\033[0m"  # red
    elif side == "right":
        return f"\033[94m{side}\033[0m"  # blue
    return side


class River_State:
    def __init__(self, traveler, food, dog, koala):
        self.traveler = traveler
        self.food = food
        self.dog = dog
        self.koala = koala

    def __str__(self):
        return (
            f"(Traveler:{colorize(self.traveler)}, "
            f"Food:{colorize(self.food)}, "
            f"Dog:{colorize(self.dog)}, "
            f"Koala:{colorize(self.koala)})"
        )

    def illegal(self):
        if (self.food == self.dog) and (self.food != self.traveler):
            return True
        if (self.food == self.koala) and (self.food != self.traveler):
            return True
        return False

    def equals(self, state):
        return (
            self.traveler == state.traveler
            and self.food == state.food
            and self.dog == state.dog
            and self.koala == state.koala
        )

    def operator_names(self):
        actions = []
        actions.append("move_alone")
        if self.traveler == self.food:
            actions.append("move_food")
        if self.traveler == self.dog:
            actions.append("move_dog")
        if self.traveler == self.koala:
            actions.append("move_koala")
        return actions

    def move_alone(self):
        if self.traveler == "left":
            new_side = "right"
        else:
            new_side = "left"
        return River_State(new_side, self.food, self.dog, self.koala)

    def move_food(self):
        if self.traveler == "left":
            new_side = "right"
        else:
            new_side = "left"
        return River_State(new_side, new_side, self.dog, self.koala)

    def move_dog(self):
        if self.traveler == "left":
            new_side = "right"
        else:
            new_side = "left"
        return River_State(new_side, self.food, new_side, self.koala)

    def move_koala(self):
        if self.traveler == "left":
            new_side = "right"
        else:
            new_side = "left"
        return River_State(new_side, self.food, self.dog, new_side)

    def apply_operators(self):
        ops = []
        for name in self.operator_names():
            if name == "move_alone":
                ops.append(self.move_alone())
            elif name == "move_food":
                ops.append(self.move_food())
            elif name == "move_dog":
                ops.append(self.move_dog())
            elif name == "move_koala":
                ops.append(self.move_koala())
        return ops


# Note: basicsearch.py was edited to make the print outs easier to read
Search(
    River_State("left", "left", "left", "left"),
    River_State("right", "right", "right", "right"),
)
