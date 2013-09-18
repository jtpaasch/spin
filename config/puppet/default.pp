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

# Install emacs-nox
############################################
package { "emacs-nox":
    ensure => present,
}

# Install git
############################################
package { "git":
    ensure => present,
}


