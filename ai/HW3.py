import numpy as np
import matplotlib.pyplot as plt
import random


class QLearningAgent:
    def __init__(self, rows, cols, alpha, gamma, epsilon):
        self.rows = rows
        self.cols = cols
        self.alpha = alpha
        self.gamma = gamma
        self.epsilon = epsilon

        # 3D cube rows, cols, actions
        self.q_table = np.zeros((rows, cols, 4))

        # 0: Up, 1: Down, 2: Left, 3: Right
        self.actions = [0, 1, 2, 3]

    def choose_action(self, state):
        row, col = state

        if random.uniform(0, 1) < self.epsilon:
            return random.choice(self.actions)
        else:
            action_values = self.q_table[row, col]  # values for all 4 actions
            return np.argmax(action_values)  # action with highest value

    def update_q_table(
        self, state, action, reward, next_state_coordinates, finally_done
    ):
        row, col = state
        next_row, next_col = next_state_coordinates

        current_q = self.q_table[row, col, action]  # single

        if finally_done:
            next_max_q = 0
        else:
            next_max_q = np.max(self.q_table[next_row, next_col])  # max of next state

        target_q = reward + self.gamma * next_max_q

        self.q_table[row, col, action] = current_q + self.alpha * (target_q - current_q)


class CliffGridworld:
    def __init__(self):
        self.rows = 4
        self.cols = 10

        self.start_state = (3, 0)
        self.goal_state = (3, 9)

        # cliffs
        self.cliffs = [(3, 1), (3, 2), (3, 3), (3, 4), (3, 5), (3, 6), (3, 7), (3, 8)]

        self.action_coordinate_update = [(-1, 0), (1, 0), (0, -1), (0, 1)]

    def reset(self):
        return self.start_state

    def step(self, state, action):

        d_row, d_col = self.action_coordinate_update[action]

        next_row = state[0] + d_row
        next_col = state[1] + d_col

        next_row = max(0, min(next_row, self.rows - 1))
        next_col = max(0, min(next_col, self.cols - 1))

        next_state_coordinates = (next_row, next_col)

        if next_state_coordinates == self.goal_state:
            reward = 100
            finally_done = True
        elif next_state_coordinates in self.cliffs:
            reward = -100
            next_state_coordinates = self.start_state
            finally_done = False
        else:
            reward = -1
            finally_done = False

        return next_state_coordinates, reward, finally_done


class WindyGridworld:
    def __init__(self):
        self.rows = 7
        self.cols = 10

        self.start_state = (3, 0)
        self.goal_state = (3, 7)

        self.wind = [0, 0, 0, 1, 1, 1, 2, 2, 1, 0]

        self.action_coordinate_update = [(-1, 0), (1, 0), (0, -1), (0, 1)]

    def reset(self):
        return self.start_state

    def step(self, state, action):
        if state == self.goal_state:
            return state, 0, True

        d_row, d_col = self.action_coordinate_update[action]

        next_row = state[0] + d_row - self.wind[state[1]]
        next_col = state[1] + d_col

        next_row = max(0, min(next_row, self.rows - 1))
        next_col = max(0, min(next_col, self.cols - 1))

        next_state_coordinates = (next_row, next_col)

        if next_state_coordinates == self.goal_state:
            reward = 100
            finally_done = True
        else:
            reward = -1
            finally_done = False

        return next_state_coordinates, reward, finally_done


def train_agent(
    grid,
    agent,
):
    trial_entries = []

    for _ in range(100):
        current_state = grid.reset()
        total_reward = 0
        finally_done = False

        for _ in range(500):
            action = agent.choose_action(current_state)
            next_state_coordinates, reward, finally_done = grid.step(
                current_state, action
            )

            agent.update_q_table(
                current_state, action, reward, next_state_coordinates, finally_done
            )

            total_reward += reward
            current_state = next_state_coordinates

            if finally_done:
                break

        trial_entries.append(total_reward)

    return trial_entries


if __name__ == "__main__":
    print("Cliff Gridworld")

    cliff_grid = CliffGridworld()
    cliff_agent = QLearningAgent(
        rows=cliff_grid.rows,
        cols=cliff_grid.cols,
        alpha=0.5,
        gamma=0.99,
        epsilon=0.1,
    )

    cliff_rewards = train_agent(cliff_grid, cliff_agent)
    plt.figure(figsize=(12, 6))
    plt.plot(cliff_rewards, label="Trial Reward")
    plt.title("Q-Learning on Cliff Gridworld")
    plt.xlabel("Trial")
    plt.ylabel("Total Reward per Trial")
    plt.legend()
    plt.grid(True)
    plt.show()
    print("training done")

    print(" Windy Gridworld")

    windy_grid = WindyGridworld()
    windy_agent = QLearningAgent(
        rows=windy_grid.rows,
        cols=windy_grid.cols,
        alpha=0.5,
        gamma=0.99,
        epsilon=0.1,
    )

    windy_rewards = train_agent(windy_grid, windy_agent)
    plt.figure(figsize=(12, 6))
    plt.plot(windy_rewards, label="Trial Reward")
    plt.title("Q-Learning on Windy Gridworld")
    plt.xlabel("Trial")
    plt.ylabel("Total Reward per Trial")
    plt.legend()
    plt.grid(True)
    plt.show()
    print("training done")
