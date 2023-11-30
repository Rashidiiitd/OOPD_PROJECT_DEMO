# OOPD_PROJECT_DEMO
# ls functionalities 
Sure, let's break down each of the `ls` commands you've mentioned:

1. **`ls -S`**: This command lists files and directories in the current directory, sorting them by size in descending order. The largest files or directories will appear first.

2. **`ls -h`**: This command lists files and directories in the current directory, making the sizes human-readable. This means file sizes will be displayed in a more easily understandable format, using units such as KB (kilobytes), MB (megabytes), GB (gigabytes), etc.

3. **`ls -t`**: This command lists files and directories in the current directory, sorting them by modification time with the newest files first. The `-t` flag stands for "time".

4. **`ls -l`**: This command lists files and directories in a detailed, long format. It provides additional information about each file, such as permissions, owner, group, size, and modification time.
5  **`ls -R`**: This command is used to print the directerios in recursive manner



Each of these options can be used individually or combined based on your specific requirements when using the `ls` command in a Unix-like shell.
# cd functionalities 

1. `cd ~`: This command changes the current directory to your home directory. The tilde (`~`) represents the home directory.

2. `cd ..`: This command moves up one level in the directory hierarchy. It's like going to the parent directory.

3. `cd -`: This command switches to the previous directory you were in. It's a quick way to toggle between two directories.

4. `cd [directory_path]`: This command changes the current directory to the one specified in `[directory_path]`. You replace `[directory_path]` with the actual path you want to navigate to.

5. `ls`: This command lists the contents of the current directory.

So, if you put them together like `cd [directory_path] && ls`, it means change to the specified directory and then list its contents. The `&&` ensures that the second command (`ls`) will only run if the first one (`cd [directory_path]`) is successful.
