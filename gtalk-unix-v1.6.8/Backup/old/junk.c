      {
	int fd;

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	if ((fd=open(mynode->device->name, O_RDWR|O_NOCTTY)) < 0)
	  {
	    log_error("Could not reopen modem %s\n",mynode->device->name);
	    return (-1);
	  }
	if (ioctl(fd, TIOCSCTTY, (char *) 0) < 0)
	  {
	    log_error("Could not reestablish a controlling terminal\n");
	    return (-1);
	  }
	if (fd != STDIN_FILENO)
	  if (dup2(fd, STDIN_FILENO) != STDIN_FILENO)
	    return (-1);
	if (fd != STDOUT_FILENO)
	  if (dup2(fd, STDOUT_FILENO) != STDOUT_FILENO)
	    return (-1);
	if (fd != STDERR_FILENO)
	  if (dup2(fd, STDERR_FILENO) != STDERR_FILENO)
	    return (-1);
	if (fd > STDERR_FILENO)
	  close(fd);
      }
