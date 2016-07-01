The tizen-app-assist project uses the signed-off-by language and process, to give us a clear chain of trust for every patch received.

> By making a contribution to this project, I certify that:  

> (a)	The contribution was created in whole or in part by me and I have the right to submit it under the open source license indicated in the file; or

> (b)	The contribution is based upon previous work that, to the best of my knowledge, is covered under an appropriate open source license and I have the right under that license to submit that work with modifications, whether created in whole or in part by me, under the same open source license (unless I am permitted to submit under a different license), as indicated in the file; or

> (c)	The contribution was provided directly to me by some other person who certified (a), (b) or (c) and I have not modified it.

> (d)	I understand and agree that this project and the contribution are public and that a record of the contribution (including all personal information I submit with it, including my sign-off) is maintained indefinitely and may be redistributed consistent with this project, under the same open source license. 


### Using the Signed-Off-By Process

We have the same requirements for using the signed-off-by process as the Linux kernel. In short, you need to include a signed-off-by tag in every patch:

"Signed-off-by:" this is a developer's certification that he or she has the right to submit the patch for inclusion into the project. It is an agreement to the Developer's Certificate of Origin (above). **Code without a proper signoff cannot be merged into the mainline.**

You should use your real name and email address in the format below:

> tizen-app-assist-DCO-1.0-Signed-off-by: John Doe johndoe@example.org


#### How to add DCO every single commit automatically.

It is easy to forget adding DCO end of every commit message. Fortunately there is a nice way to do it automatically. Once you've clone the repository into your local machine, you can add `prepare commit message hook` in `.git/hooks` directory like this:

```
#!/usr/bin/env python

import sys

commit_msg_filepath = sys.argv[1]

with open(commit_msg_filepath, "r+") as f:
	content = f.read()
	f.seek(0, 0)
	f.write("%s\n\ntizen-app-assist-DCO-1.0-Signed-off-by: <Your Name> <Your Email>" % content)
```

Please refer [Git Hooks](http://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks) for more information.