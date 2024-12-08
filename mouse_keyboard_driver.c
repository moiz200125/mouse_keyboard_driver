#include <linux/module.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/keyboard.h>


#define R_KEY 64370
#define L_KEY 64364
static struct input_dev *virtual_mouse;

/**
 * Keyboard notifier callback to intercept keypresses
 */
static int keyboard_notifier_callback(struct notifier_block *nb, unsigned long action, void *data)
{
    struct keyboard_notifier_param *param = data;
    if (action == KBD_KEYSYM && param->down)
    { // Only process key press events
        printk(KERN_INFO "Action on keyboard: %d\n", param->value);
        if (param->value == R_KEY) // right keyboard
        {   
            printk(KERN_INFO "Right key pressed!\n");                                               // R key
            input_report_key(virtual_mouse, BTN_RIGHT, 1); // Press right mouse button
            input_sync(virtual_mouse);
            input_report_key(virtual_mouse, BTN_RIGHT, 0); // Release right mouse button
            input_sync(virtual_mouse);
        }
        else if (param->value == L_KEY)
        {
            printk(KERN_INFO "Left key pressed!\n");                                             // L key
            input_report_key(virtual_mouse, BTN_LEFT, 1); // Press left mouse button
            input_sync(virtual_mouse);
            input_report_key(virtual_mouse, BTN_LEFT, 0); // Release left mouse button
            input_sync(virtual_mouse);
        }
    }
    return NOTIFY_OK;
}

/* Notifier block to register for keyboard events */
static struct notifier_block keyboard_notifier = {
    .notifier_call = keyboard_notifier_callback};

/**
 * Module initialization function
 */
static int __init keyboard_to_mouse_init(void)
{
    int ret;

    // Allocate virtual input device for mouse simulation
    virtual_mouse = input_allocate_device();
    if (!virtual_mouse)
    {
        pr_err("Failed to allocate virtual input device\n");
        return -ENOMEM;
    }

    // Configure the virtual mouse device
    virtual_mouse->name = "Keyboard-to-Mouse Mapper";
    virtual_mouse->evbit[0] = BIT_MASK(EV_KEY);
    set_bit(BTN_LEFT, virtual_mouse->keybit);
    set_bit(BTN_RIGHT, virtual_mouse->keybit);

    // Register the virtual input device
    ret = input_register_device(virtual_mouse);
    if (ret)
    {
        pr_err("Failed to register virtual input device\n");
        input_free_device(virtual_mouse);
        return ret;
    }

    // Register the keyboard notifier
    register_keyboard_notifier(&keyboard_notifier);
    pr_info("Keyboard-to-Mouse module loaded\n");
    return 0;
}

/**
 * Module cleanup function
 */
static void __exit keyboard_to_mouse_exit(void)
{
    unregister_keyboard_notifier(&keyboard_notifier);
    input_unregister_device(virtual_mouse);
    pr_info("Keyboard-to-Mouse module unloaded\n");
}

module_init(keyboard_to_mouse_init);
module_exit(keyboard_to_mouse_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A kernel module to map keyboard keys to mouse buttons");
