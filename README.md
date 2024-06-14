# Installing procedures

1. Install [<u>VirtualBox</u>](https://www.virtualbox.org/)

2. Download zipped [<u>MINIX-LCOM</u>](https://drive.google.com/file/d/1dM32zTzkTUEqNtTwsO-n__XO6r1Y5I-f/view) image 

3. Extract the image from the zip

4. Open the .vbox file using Virtual Box
    * double-clicking the file should execute Virtual Box with the settings for the MINIX-LCOM VM

5. You need to share a folder under the MINIX-LCOM/ folder that you unzipped above. Create a directory (e.g.: shared) for this.

6. Clone the repository in the shared folder
```bash
$ git clone git@github.com:YZ1GO/L.EIC018_LC_PRJ.git
```

# How to run
1. You can now start the VM. The VM should start even if the shared folder configuration is not fixed. It will issue a warning, but this does not prevent Minix from booting. Of course, there will be no shared folders.

* The credentials for login are: <strong>lcom</strong> with password <strong>lcom</strong>.


