# cellular_automata
An Object-Oriented implementation of cellular automata with applications to vehicular flow and biological flow.

## How to become a collaborator?

### Let us know you want to become a collaborator to give you credit for your work

1. Send us an email (jcp.sansalvador at inaoe.mx) to let us include you in our list of collaborators and grant you access to the repository.

### Get the code

2. Get a copy of the code by cloning it in your local computer and start working at your own pace

```basch
git clone https://github.com/tachidok/cellular_automata/
```
2.1 You will be prompted with your github user name and your password
2.2 Once you have a local copy at your computer go into the `cellullar_automata` folder by typing

```basch
cd cellular_automata
```

2.3 Once in the `cellular_automata` folder create a new branch with your name so you can freely develop your own version of the framework, do this by typing

```basch
git checkout -b john
```

to create a new branch with the name `john`

2.4 Install the framework and check the documentation

```basch
./autogen.sh
```

2.4.1 To install the framework follow the instructions on screen after typing the above command, you will need a recent `cmake` version and your preferred `C++` compiler

### Merge your contribution with the master branch

3. Once you are done with your work add the modified files to track their history and do a commit

```basch
git add your_modified_files
git commit
```

4. Once you are done with all your changes and commits you can merge your branch with the `master` branch, do this by typing

```basch
git checkout master
git merge john
```







