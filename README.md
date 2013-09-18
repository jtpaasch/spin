Spin
====

This is a simple command line tool that lets *nix users spin up VMs quickly and easily. It is really just a wrapper for vagrant and puppet. (It is much like [Spinup.py](https://github.com/jpaasch/Spinup.py), just simpler and, for me anyway, more convenient).


Requirements
------------

- a *nix system.
- vagrant 1.2 (vagrant 1.1 will not work!)
- puppet
- git


Installation
------------

Download or clone this repo, navigate into the main folder, and then run the `make` command to compile the executable: 

    $ make

Then to install it, just run the `make install` command:

    $ make install

That's it. Now the command `spin` is available.


Usage
-----

To create a VM, create a folder to house it, and navigate into that folder:

    $ mkdir my-vm
    $ cd my-vm

Now, you simply need to run the `spin up` command:

    $ spin up

If there is no VM in the folder, `spin` will create one before booting it up. 

To SSH into the VM once it's up and running, use the `spin ssh` command:

    $ spin ssh

To shut the VM down, use the `spin down` command:

    $ spin down

To remove the VM and all its config files from your system, use the `spin destroy` command:

    $ spin destroy


Local config
------------

For each VM, `spin` stores its config files in a hidden folder called `.devbox`. You can see the contents of that folder if you like:

    $ cd my-vm
    $ ls -la my-vm/.devbox

The vagrant and puppet files are stored in their respective folders:

    my-vm/.devbox/vagrant/Vagrantfile
    my-vm/.devbox/puppet/default.pp

If you want to edit the configuration and provision your VM in a particular way, edit those files. You can do the same for any other VM you've created. Just go into its .devbox folder and edit the relevant files.


Global config
-------------

Default (global) configuration files are stored in a `.spin` folder, which resides in your home directory:

    $ cd ~/.spin
    $ ls -la 

If you change the configuration here, `spin` will use it for any new VMs it creates. This will not affect any VMs you've already created. Only new VMs you create going forward.



