#include <lcom/lcf.h>
#include "graphic.h"

static void *video_mem; 
static vbe_mode_info_t vbe_mem_info;
//static mmap_t mem_map;

/*int (vg_exit)() {
  reg86_t r86; 
  
  memset(&r86, 0, sizeof(r86));	

  r86.intno = 0x10; 
  r86.ah = 0x00;    
  r86.al = 0x03;    
  
  if( sys_int86(&r86) != OK ) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return 1;
  }
  return 0;
}*/

int (set_graphics_mode)(uint16_t mode) {
    struct reg86 reg_86;

    memset(&reg_86, 0, sizeof(struct reg86)); 

    // Set Reg86
    reg_86.intno = VC_BIOS_SERV;
    reg_86.ah = VBE_CALL; //0x4F
    reg_86.al = SET_VBE_MD; //0x02
    reg_86.bx = mode | LINEAR_FRAME_BUFFER_MD; // mode | 1<<14

    // BIOS CALL
    if (sys_int86(&reg_86) != OK) {
        printf("%s: sys_int86 failed\n", __func__);
        return 1;
    }
    return 0;
}

int (get_permission)(unsigned int base_addr, unsigned int size) {
    struct minix_mem_range mmr;
    mmr.mr_base = base_addr;
    mmr.mr_limit = base_addr + size;
    return sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mmr);
}

phys_bytes get_phys_addr(void) {
    return vbe_mem_info.PhysBasePtr;
}

unsigned int get_vram_size(void) {
    return vbe_mem_info.XResolution * vbe_mem_info.YResolution * ((vbe_mem_info.BitsPerPixel + 7) >> 3);
}

uint16_t get_XRes(void) {
    return vbe_mem_info.XResolution;
}

uint16_t get_YRes(void) {
    return vbe_mem_info.YResolution;
}

uint16_t get_bits_pixel(void) {
    return vbe_mem_info.BitsPerPixel;
}

uint16_t get_bytes_pixel(void) {
    return (vbe_mem_info.BitsPerPixel + 7) >> 3;
}

uint16_t get_RedMaskSize  (void){ return vbe_mem_info.RedMaskSize  ; }
uint16_t get_GreenMaskSize(void){ return vbe_mem_info.GreenMaskSize; }
uint16_t get_BlueMaskSize (void){ return vbe_mem_info.BlueMaskSize ; }

int (map_vram)(void) {
    int r;
    unsigned int vram_base = get_phys_addr();
    unsigned int vram_size = get_vram_size();
    if ((r = get_permission(vram_base, vram_size))) {
        panic("%s: sys_privctl (ADD MEM) failed: %d\n", __func__, r);
    }

    video_mem = vm_map_phys(SELF, (void *)vram_base, vram_size);

    if (video_mem == MAP_FAILED) {
        panic("%s: couldn't map video memory.", __func__);
    }

    return 0;
}

void* (vg_init)(uint16_t mode) {
    vbe_get_mode_info(mode, &vbe_mem_info);
    map_vram();
    set_graphics_mode(mode);
    return video_mem;
}


//mem video + (x + y*hres)*bp

int (set_pixel_no_black)(uint16_t x, uint16_t y, uint32_t color) {
    if (color == 0) {
        return 0;
    }
    if (x > vbe_mem_info.XResolution || y > vbe_mem_info.YResolution) {
        return 1;
    }
    unsigned int pos = (x + y * vbe_mem_info.XResolution) * get_bytes_pixel();
    if (memcpy((void*)((unsigned int)video_mem + pos), &color, get_bytes_pixel()) == NULL) {
        return 1;
    }
    return 0;
}

int (set_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    if (x > vbe_mem_info.XResolution || y > vbe_mem_info.YResolution || x < 0 || y < 0) {
        return 0;
    }
    unsigned int pos = (x + y * vbe_mem_info.XResolution) * get_bytes_pixel();
    if (memcpy((void*)((unsigned int)video_mem + pos), &color, get_bytes_pixel()) == NULL) {
        return 1;
    }
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (uint16_t i = 0; i < len; i++) {
        if (set_pixel(x + i, y, color)) {
            return 1;
        }
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (uint16_t j = 0; j < height; j++) {
        if (vg_draw_hline(x, y + j, width, color)) {
            return 1;
        }
    }
    return 0;
}
