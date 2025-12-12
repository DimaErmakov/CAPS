import itertools

# Common file names related to the exam
common_files = [
    "exam",
    "flag",
    "leaked_exam",
    "test",
    "challenge",
    "secret",
    "quiz",
]

# Common file extensions
common_extensions = [
    "", ".txt", ".bak", ".backup", ".tmp", ".temp", ".log", ".conf", ".cfg",
    ".sh", ".py", ".old", ".md", ".doc", ".pdf", ".zip",
]

# Directories to check
directories = [
    "/home/restrictedstudent/secret/",
    "/home/restrictedstudent/secret/exam",
    "/home/restrictedstudent/secret/exam/",
    "/home/restrictedstudent/.bash/",
    "/home/restrictedstudent/./",
    "/home/restrictedstudent/../",
    "/home/restrictedstudent/.bash_profile/",
    "/home/restrictedstudent/bin/",
    "/home/restrictedstudent/bin/exam",
    "/home/restrictedstudent/bin/exam/",
    # "/tmp/"
]

# Commands to attempt
command_templates = [
    "cat {path}",
    "./{path}",
    "stat {path}",
    "ls -l {path}",
    "file {path}",
    "head -n 5 {path}",
]

# Additional system commands to check history and environment
system_commands = [
    "history | grep -i exam",
    "cat ~/.bash_history | grep -i exam",
    "ls -la /tmp",
    "find /tmp -name '*exam*' 2>/dev/null",
    "find /tmp -name '*secret*' 2>/dev/null",
    "groups",
    "ls -ld /home/restrictedstudent/secret",
    "ls -ld /home/restrictedstudent/bin",
    "env | grep -i restricted",
    "ls -la /home/dermakov/",
    "cat /home/restrictedstudent/.bashrc",
    "cat /home/restrictedstudent/.bash_history",
    "find /home/restrictedstudent -name '*bak' 2>/dev/null",
]

# Generate possible file paths
file_paths = []
for directory, file_name, ext in itertools.product(directories, common_files, common_extensions):
    file_paths.append(f"{directory}{file_name}{ext}")

# Generate commands for each file path
commands = []
for path in file_paths:
    for template in command_templates:
        commands.append(template.format(path=path))

# Add system commands
commands.extend(system_commands)

# Save commands to a text file
output_file = "exam_search_commands.txt"
with open(output_file, "w") as f:
    for cmd in commands:
        f.write(f"{cmd}\n")

print(f"Generated {len(commands)} commands and saved to '{output_file}'.")
print(f"You can manually run commands from '{output_file}' or use it as input for a script.")
print("Note: Some commands may fail due to permissions. Review output for clues.")