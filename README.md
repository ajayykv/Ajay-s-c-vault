LSb steganography is the method of hiding a secret file within an image by replacing the "least significant bit" of each pixel's color value with a bit from the data inside secret file. 
The advantage of this method is that changing lsb bit has a negligible impact on the image's overall color, making the change unnoticeable to the human eye. 
We use bitmap image to hide the data, why because bitmap (.bmp) format image will be not compressed when you share the image and we will not lose the secret message inside the image due to compression. 
You can pass the bitmap source image and secret file to the program as command line arguments. 

The project performs two operations :-
Encoding : Hiding the secret file inside user given source image and thus producing a resultant stego.bmp image. Here you encode magic string, file extension, file size and actual data.
Decoding : Extracting the secret data and reproducing the secret file from the stego image file. Here you decode magic string (that acts like a passkey to this encoding method), file extension, file size and secret data.
