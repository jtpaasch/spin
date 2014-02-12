Spin
====

This is a simple command line tool for spinning up VMs quickly and easily. It is really just a wrapper for vagrant and ansible, just simpler. 


Requirements
------------

- a *nix system
- virtualbox
- vagrant 1.2+
- ansible
- git


Installation
------------

Download or clone this repo, navigate into the main folder, and then run the `make` command to compile the executable: 

    $ make

Then to install it, run the `make install` command:

    $ make install

That's it. Now the command `spin` is available.


Usage
-----

To create a VM, make a folder to house it, and navigate into that folder:

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
    $ ls -la .devbox

The vagrant and ansible files are stored in their respective folders:

    my-vm/.devbox/vagrant/Vagrantfile
    my-vm/.devbox/ansible/playbook.yml

If you want to edit the configuration and provision your VM in a particular way, edit those files. You can do the same for any other VM you've created. Just go into its `.devbox` folder and edit the relevant files.


Global config
-------------

By default, `spin` creates CentOS 6.4 VMs. You can change that if you like. 

The default/global configuration files are stored in a `.spin` folder, which resides in your home directory:

    $ cd ~/.spin
    $ ls -la 

Once you change the configuration there, `spin` will use it for any new VM it creates. This will not affect any VMs you've already created.



