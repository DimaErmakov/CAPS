import random
import os


class VacuumWorld:

    def __init__(self):
        self.width = 10
        self.height = 10
        # 0 = clean, 1 = dirt
        self.grid = []
        self.total_reward = 0

        self.reset()

    def reset(self):
        self.grid = []

        for _ in range(self.height):
            row = []

            for _ in range(self.width):
                rand_value = random.random()

                if rand_value < 0.25:
                    cell_value = 1
                else:
                    cell_value = 0

                row.append(cell_value)
            self.grid.append(row)

        # rand pos
        self.agent_pos = {
            "row": random.randint(0, self.height - 1),
            "col": random.randint(0, self.width - 1),
        }
        self.total_reward = 0

    def print_world(self):
        os.system("cls")  # clear
        for r in range(self.height):
            row_str = ""
            for c in range(self.width):
                if r == self.agent_pos["row"] and c == self.agent_pos["col"]:
                    row_str += " V "
                elif self.grid[r][c] == 1:
                    row_str += " D "
                else:
                    row_str += " . "  # clean
            print(row_str)
        print("-" * 30)
        print(f"Total Reward: {self.total_reward}")
        print("-" * 30)

    def perform_action(self, action):
        reward = 0
        current_row, current_col = self.agent_pos["row"], self.agent_pos["col"]

        if action == "up":
            reward = -1
            if current_row > 0:
                self.agent_pos["row"] -= 1
        elif action == "down":
            reward = -1
            if current_row < self.height - 1:
                self.agent_pos["row"] += 1
        elif action == "left":
            reward = -1
            if current_col > 0:
                self.agent_pos["col"] -= 1
        elif action == "right":
            reward = -1
            if current_col < self.width - 1:
                self.agent_pos["col"] += 1
        elif action == "clean":
            if self.grid[current_row][current_col] == 1:
                # clean dirty
                reward = 3
                self.grid[current_row][current_col] = 0
            else:
                # attemp to clean a clean
                reward = -1

        return reward

    def run_simulation(self, steps=50, mode="random"):
        self.reset()

        for step in range(0, steps):
            self.print_world()
            print(f"Step {step+1}/{steps}")

            available_actions = ["up", "down", "left", "right", "clean"]
            print(f"Available Actions: {available_actions}")

            chosen_action = ""
            if mode == "user":
                while chosen_action not in available_actions:
                    chosen_action = input("Choose an action: ").lower()
                    if chosen_action not in available_actions:
                        print(f"Invalid action. Please choose from the list.")
            else:  # rand
                chosen_action = random.choice(available_actions)
                print(f"Randomly Chosen Action: {chosen_action}")

            step_reward = self.perform_action(chosen_action)
            self.total_reward += step_reward

            print(f"Action: '{chosen_action}', Reward: {step_reward}")
            if mode == "random":
                input("Press Enter to continue to the next step...")

        self.print_world()
        print("Simulation Complete")
        print(f"Final Total Reward after {steps} steps: {self.total_reward}")


def main():
    world = VacuumWorld()
    while True:
        mode_choice = int(input("Choose a mode: user = 1 or random = 2\n"))
        if mode_choice == 1:
            world.run_simulation(steps=50, mode="user")
        elif mode_choice == 2:
            world.run_simulation(steps=50, mode="random")
        else:
            print("Invalid input")


if __name__ == "__main__":
    main()
