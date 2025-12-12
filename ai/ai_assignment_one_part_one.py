import random


class StateSpaceModel:
    def __init__(self, states, actions, rewards, transitions):
        self.states = states
        self.actions = actions
        self.rewards = rewards
        self.transitions = transitions
        self.current_state = None
        self.total_reward = 0

    def get_available_actions(self, state):
        if state in self.transitions:
            actions = list(self.transitions[state].keys())
            return actions
        else:
            return []

    def get_reward(self, state, action):
        key = (state, action)
        if key in self.rewards:
            reward = self.rewards[key]
            return reward
        else:
            return 0

    def get_next_state(self, state, action):
        if state in self.transitions:
            state_transitions = self.transitions[state]
            if action in state_transitions:
                next_states_prob = state_transitions[action]
            else:
                next_states_prob = {}
        else:
            next_states_prob = {}

        if not next_states_prob:
            return state

        next_states = list(next_states_prob.keys())
        probabilities = list(next_states_prob.values())
        chosen_state = random.choices(next_states, weights=probabilities, k=1)[0]
        return chosen_state

    def run_simulation(self, steps=10, mode="random"):
        self.current_state = random.choice(self.states)
        self.total_reward = 0

        print(f"\nStarting Simulation, Mode: {mode}")
        print(f"Initial State: {self.current_state}\n")

        for step in range(1, steps + 1):
            print(f"Step {step}/{steps}")
            print(f"Current State: {self.current_state}")

            available_actions = self.get_available_actions(self.current_state)

            if not available_actions:
                print("No actions available")
                break

            print(f"Available Actions: {available_actions}")

            chosen_action = ""
            if mode == "user":
                while chosen_action not in available_actions:
                    chosen_action = input("Choose an action: ")
                    if chosen_action not in available_actions:
                        print(f"Invalid action, choose from {available_actions}")
            else:
                chosen_action = random.choice(available_actions)
                print(f"Random Chosen Action: {chosen_action}")

            step_reward = self.get_reward(self.current_state, chosen_action)
            self.total_reward += step_reward

            next_state = self.get_next_state(self.current_state, chosen_action)

            print(f"Reward for this step: {step_reward}")
            print(f"Total Reward: {self.total_reward}")

            self.current_state = next_state
            print(f"Next State: {self.current_state}\n")

        print(f"Final State: {self.current_state}")
        print(f"Final Total Reward: {self.total_reward}")


def main():
    states = ["s1", "s2", "s3", "s4"]
    actions = ["a1", "a2", "a3"]

    rewards = {
        ("s1", "a1"): 2,
        ("s1", "a2"): 1,
        ("s2", "a3"): 1,
        ("s3", "a1"): 2,
        ("s3", "a2"): 3,
        ("s4", "a1"): 1,
        ("s4", "a2"): 2,
        ("s4", "a3"): 1,
    }

    transitions = {
        "s1": {"a1": {"s2": 0.5, "s4": 0.5}, "a2": {"s3": 1.0}},
        "s2": {"a3": {"s1": 0.25, "s4": 0.75}},
        "s3": {"a1": {"s1": 1.0}, "a2": {"s2": 0.4, "s4": 0.6}},
        "s4": {"a1": {"s1": 1.0}, "a2": {"s2": 1.0}, "a3": {"s3": 1.0}},
    }

    model = StateSpaceModel(states, actions, rewards, transitions)

    while True:
        mode_choice = int(input("Choose a mode: user = 1 or random = 2\n"))
        if mode_choice == 1:
            model.run_simulation(steps=10, mode="user")
        elif mode_choice == 2:
            model.run_simulation(steps=10, mode="random")
        else:
            print("Invalid input")


if __name__ == "__main__":
    main()
