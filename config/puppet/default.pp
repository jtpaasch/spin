#######################################################################
#
#    PUPPET MANIFEST
#
#    Provisions a vagrant box VM.
#    Copyright 2013 JT Paasch
#


# Set the PATH
#######################################
Exec {
    path => [
        "/usr/bin", 
        "/usr/sbin", 
        "/sbin", 
        "/bin", 
        "/usr/local/bin", 
        "/usr/local/sbin"
    ],
}

# Install vim
############################################
package { "vim":
    ensure => latest,
}

# Install emacs-nox
############################################
package { "emacs-nox":
    ensure => latest,
}

# Install git
############################################
package { "git":
    ensure => latest,
}


