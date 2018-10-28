#include "vc.h"
#include "lib.h"
#include "keyboard.h"
#include "video.h"

/*
 * init_vc
 * Description: Initialize the virtual console to be used.
 * Input: none
 * Output: none
 * Side effects: screen is cleared. a new virtual console is created is created.
 * Return: none
 */

void init_vc(void){
    cli();
    clear();  /* FIXME change to our clear */
    update_cursor(0,0);
    int i;
    for(i = 0; i < BUFFER_SIZE; i++){
      vc_buffer[i] = '\0';
    }
    sti();
}
/*
 * vc_open
 * Description: Open the virtual console file
 * Input: Garbage
 * Output: None
 * Side effects: clears screen
 * Return: 0 on success
 */
int32_t vc_open(void * buf, uint32_t bytes){
  init_vc();
  return 0;
}
/*
 * vc_close
 * Description: Close the virtual console
 * Input: None
 * Output: None
 * Side effects: None
 * Return: 0
 */
int32_t vc_close(void * buf, uint32_t bytes){

  // if(curr_vc_id == 0)
  //     return -1;
  // curr_vc_id--;
  // if(curr_vc_id != 0)
  //     vc_load(curr_vc_id);
  return 0;
}

/*
 * vc_write
 * Description: Writes a given number of bytes from a buffer to the terminal
 * Input: Pointer to the buffer and number of bytes to be written.
 * Output: Written to virtual memory
 * Side effects: Changes the screen cursor position and video memory.
 * Return: -1 on failure, 0 on success
 */

int32_t vc_write(void * buf, uint32_t bytes){
  if(buf == NULL)
      return -1;
  int i;
  if (bytes > BUFFER_SIZE)
      bytes = BUFFER_SIZE; /* maximum nmber of bytes we can print to the screen */
  char * buffer = (char * )buf;
  cli();

  print_term((uint32_t *)buffer, bytes);

  sti();
  return 0;
}

/*
 * vc_read
 * Description: Copies the number of bytes specified to a specified buffer
 * Input: Pointer to buffer to be modified. Number of bytes to be copied
 * Output: Returns 0 on success. -1 on failure.
 * Side effects: Buffer is cleared
 * Return: 0 on success, -1 on failure
 */

int32_t vc_read(void * buf, uint32_t bytes){

    if(buf == NULL)
        return -1;
    int i;
    if(bytes > BUFFER_SIZE)
        bytes = BUFFER_SIZE; /* maximum number of bytes we can read */
    char* buffer =  (char *)buf;

    while(vc_buffer[0] == '\0');

    for(i = 0; i < bytes; i++){
        buffer[i] = vc_buffer[i];
    }

    clr_buf();
    return 0;
}

/*
 * clr_buf
 * Description: Helper function to clear the vc_buffer
 * Input: none
 * Output: none
 * Side effects: vc_buffer emptied
 * Return: none
 */

void clr_buf(){
    int i;
    for(i = 0; i < BUFFER_SIZE; i++){
        vc_buffer[i] = '\0';
    }
}


// void load_vc(uint8_t vc_id){
//
// }
//
// void copy_vc(uint8_t vc_id){
//
// }
//
// void rstr_vc(uint8_t vc_id){
//
// }


char * get_buffer(){
  return vc_buffer;
}


/* REFERENCE: https://wiki.osdev.org/Text_Mode_Cursor */

/*
 * update_cursor
 * Description: Updates position of cursor on the screen
 * Input: takes in position on VGA, integer x and y
 * Output: cursor location in VGA
 * Side effects: overwrites current cursor location
 * Return: none
 */
void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));

}