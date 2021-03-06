#include "lib.h"
#include "filesys.h"
#include "multiboot.h"

int32_t stringcompare(const uint8_t * a, const uint8_t * b, int cmplen);
int32_t stringlength(const uint8_t * string);
void fnamecopy(const uint8_t * source, uint8_t * dest);



/* init_filesys
 * This function is called in kernel.c and sets up the necessary
 * global pointers necessary to perform operations on the filesystem.
 * INPUTS         bb - a pointer to a boot block, which is jus the
 *                     beginning of the filesystem
 * OUTPUTS        none
 * SIDE EFFECTS   sets the values of filesys_begin, inodes_being,
 *                data_blocks_begin, and num_inodes
 */
void init_filesys(boot_block_t * bb){
      /*Find out where the filesystem, inodes, and data begins*/
      filesys_begin = bb;
      inodes_begin = (inode_t *)(filesys_begin + 1);
      num_inodes = filesys_begin->num_inodes;
      data_block_begin = (data_block_t *)(filesys_begin + 1 + num_inodes);
      return;
}

/* read_dentry_by_name
 * Takes a string as argument and finds the associated file in the file system
 * INPUTS         takes a string as argument and an empty dentry to write
 *                all the data associated with the file into
 * OUTPUTS        returns -1 when the string name is too long, returns 0
 *                on success, and returns 1 when the string isn't in the list
 * SIDE EFFECT    fills the dentry given as argument with information about
 *                the file.
 */
int32_t read_dentry_by_name(const uint8_t * fname, dentry_t * dentry){
      int i = 0;

      /*Check to ensure the name argument isn't too long*/
      if(stringlength(fname) > FNAME_MAX_LEN){
            return -1;
      }

      /*Check for NULL pointers*/
      if(fname == NULL || dentry == NULL){
            return -1;
      }

      /*Iterate through the boot block and search for the dentry with the same name*/
      while(i < MAX_DENTRY){
            /*If we have a match, copy it into dentry and leave*/
            if(!stringcompare((uint8_t *)&(filesys_begin->directory_entries[i]), fname, FNAME_MAX_LEN)){
                  fnamecopy(filesys_begin->directory_entries[i].file_name, dentry->file_name);
                  dentry->file_type = filesys_begin->directory_entries[i].file_type;
                  dentry->inode_num = filesys_begin->directory_entries[i].inode_num;
                  return 0;
            }
            i++;
      }
      /*If we haven't found it, return -1*/
      return -1;
}

/*Important note for testing: Very large file is in index 11*/

/* read_dentry_by_index
 * This function looks into the boot block entries and finds a file with
 * the entry as argument, instead of a string containing the name of
 * the file.
 * INPUTS         index - the dentry index that can be used to look up the
 *                dentry information into the boot block
 *                dentry - the location to which the dentry information is
 *                to be copied
 * OUTPUTS        returns 0 when the file can be found and -1 on error.
 */
int32_t read_dentry_by_index(uint32_t index, dentry_t * dentry){
      //check for null pointer
      if(dentry == NULL){
            return -1;
      }

      /*Check to ensure the index is in the bounds*/
      if(index > filesys_begin->num_dir_entries){
            return -1;
      }

      /*Copy the data associated with the file into the dentry pointer provided*/
      fnamecopy(filesys_begin->directory_entries[index].file_name, dentry->file_name);
      dentry->file_type = filesys_begin->directory_entries[index].file_type;
      dentry->inode_num = filesys_begin->directory_entries[index].inode_num;
      return 0;
}

/* read_data
 * This function takes a pointer to an inode and reads a certain number of bytes
 * determined by the variable length and the position determined by offset. The
 * data is grabbed by the data blocks pointed to within the inode.
 * INPUTS         inode - the inode containing information containing data
 *                        to be read
 *                offset - the offset into the file where the data is to be read
 *                buf - where the data is to be written to
 *                length - number of bytes to be read
 * OUTPUTS        returns 0 on success, and -1 on failure
 * SIDE EFFECTS   copies the number of bytes from the data blocks into buf
 */
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t * buf, uint32_t length){
      //pointer to the inode from where the data is to be ran
      inode_t * inode_ptr = (inode_t *)(inodes_begin + inode);
      int data_block_index;
      //pointer to the current data block that is to be read from
      data_block_t * data_block_ptr;
      int offset_index = offset;
      int i;
      int bytes_read = 0;

      //ensure we're not out of bounds
      if(inode > filesys_begin->num_inodes){
            return -1;
      }

      /*Check for NULL pointer*/
      if(buf == NULL){
            return -1;
      }

      //iterate through and grab the data
      for(i = 0; i < length; i++){

            //check to ensure we're not reading data that doesn't exist
            if(offset + bytes_read > inode_ptr->length){
                  break;
            }

            //find the data block
            data_block_index = inode_ptr->block_location[offset_index / FOURKB];
            data_block_ptr = (data_block_t *)(data_block_begin + data_block_index);
            //find the byte inside the data block
            buf[i] = data_block_ptr->data[offset_index % FOURKB];
            //jump to the next byte
            offset_index++;
            bytes_read++;
      }

      return bytes_read;
}

int32_t file_open(const uint8_t * fname){
      return 0;
}

/* file_write
 * Because our filesystem is read-only, this function does nothing but
 * return -1;
 */
int32_t file_write(int32_t fd, const void * buf, int32_t n_bytes){
      return -1;
}

/* file_read
 * DESCRIPTION:   file_read reads the contents of the file and saves them in
 *                a buffer.
 * INPUTS:        inode_index - the index number giving the offset in the
 *                inode list relative to the first inode
 *                offset - the offset into the file from where the data is to
 *                be read
 *                buf - the array where to store the data that is grabbed from
 *                the file
 *                nbytes - the number of bytes to grab from the file
 * OUTPUTS:       returns the number of bytes read from the file
 * SIDE EFFECTS:  reads the nbytes from the file and stores them in the buffer
 */
int32_t file_read(uint32_t inode_index, uint32_t offset, uint8_t * buf, uint32_t nbytes){
      return read_data(inode_index, offset, buf, nbytes);
}

/* file_close
 * does nothing, but returns 0 on exit.
 */
int32_t file_close(int32_t fd){
      return 0;
}

int32_t dir_open(const uint8_t * filename){
      return 0;
}

/* dir_write
 * Does nothing but return -1, because out system is read-only.
 */
int32_t dir_write(int32_t fd, const void * buf, int32_t n_bytes){
      return -1;
}

/* dir_read
 * DESCRIPTION:   dir_read looks into a directory and reads the names of
 *                all the files found within the directory as one contiguous
 *                stream of information
 * INPUTS:        offset - the offset into the list of file names within
 *                the directory
 *                buf - the array to where the data is to be written
 *                nbytes - the number of bytes to be written
 * OUTPUTS:       returns the number of bytes written from the file.
 * SIDE EFFECTS:  copies nbytes bytes from the directory file names into
 *                the buffer supplied as argument
 */
int32_t dir_read(uint32_t inode_index, uint32_t offset, uint8_t * buf, uint32_t nbytes){
      int bits_copied = 0;
      int i;
      int local_off = offset;
      int total_dentries = filesys_begin->num_dir_entries;

      //iterate through all the file names and copy them into the buffer as needed
      for(i = 0; i<nbytes && (local_off/FNAME_MAX_LEN)<total_dentries; i++){
            buf[i] = filesys_begin->directory_entries[(local_off/FNAME_MAX_LEN)].file_name[local_off % FNAME_MAX_LEN];
            local_off++;
            bits_copied++;
      }

      return bits_copied;
}

/* dir_close
 * does nothing, but returns 0 on exit
 */
int32_t dir_close(int32_t fd){
      return 0;
}

/* stringcompare
 * Compares the contents of two strings up to a given point
 * specified by int cmplen
 * INPUTS:        a and b - the two strings to compare
 *                cmplen - by how many values to compare them
 * OUTPUTS:       returns 0 when strings are equal and 1
 *                when they are different
 * SIDE EFFECTS:  NONE
 */
int32_t stringcompare(const uint8_t * a, const uint8_t * b, int cmplen){
      int i = 0;

      /*Iterate through the strings and compare*/
      while(i < cmplen){
            /*If they're different, return 1*/
            if(a[i] != b[i]){
                  return 1;
            }
            /*If we have reached the end of the strings and they are
             *equal, return 0 */
            if(a[i] == 0 || b[i] == 0){
                  return 0;
            }
            i++;
      }

      /*If we have made it through the list with no issue,
       * the two strings are equal, so we can return 0*/
      return 0;
}

/* stringlength
 * Returns the length of a null-terminated string
 * INPUTS:        string - the string whose length is to be computed
 * OUTPUTS:       the length of the string
 * SIDE EFFECTS:  NONE
 */
int32_t stringlength(const uint8_t * string){
      int32_t length;
      /*Loop through the string and count the length*/
      for(length = 0; string[length] != NULL; length++);
      /*Now return the length*/
      return length;
}

/*fnamecopy
 *This function takes two strings as argument and copies the contents
 *of source into destination. It returns nothing.
 */
void fnamecopy(const uint8_t * source, uint8_t * dest){
      int i;
      for(i = 0; i < FNAME_MAX_LEN; i++){
            dest[i] = source[i];
      }
      return;
}
