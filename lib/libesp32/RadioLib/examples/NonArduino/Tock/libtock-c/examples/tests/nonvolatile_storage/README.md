Nonvolatile Storage Test App
============================

This app writes to flash storage and reads it back to test that flash storage
is working. It requires that a
`capsules::nonvolatile_storage_driver::NonvolatileStorage` interface be provided
to userland.



Example Hail Setup
------------------

One way to provide the Driver interface with Hail looks like:

```diff
--- a/boards/hail/src/main.rs
+++ b/boards/hail/src/main.rs
@@ -74,6 +74,7 @@ struct Hail {
     ipc: kernel::ipc::IPC,
     crc: &'static capsules::crc::Crc<'static, sam4l::crccu::Crccu<'static>>,
     dac: &'static capsules::dac::Dac<'static>,
+    nv: &'static capsules::nonvolatile_storage_driver::NonvolatileStorage<'static>,
 }

 /// Mapping of integer syscalls to objects that implement syscalls.
@@ -104,6 +105,8 @@ impl Platform for Hail {
             capsules::dac::DRIVER_NUM => f(Some(self.dac)),

             kernel::ipc::DRIVER_NUM => f(Some(&self.ipc)),
+
+            27 => f(Some(self.nv)),
             _ => f(None),
         }
     }
@@ -443,6 +446,38 @@ pub unsafe fn reset_handler() {
         capsules::dac::Dac::new(&mut sam4l::dac::DAC)
     );

+    // Flash
+    let mux_flash = static_init!(
+        capsules::virtual_flash::MuxFlash<'static, sam4l::flashcalw::FLASHCALW>,
+        capsules::virtual_flash::MuxFlash::new(&sam4l::flashcalw::FLASH_CONTROLLER));
+    hil::flash::HasClient::set_client(&sam4l::flashcalw::FLASH_CONTROLLER, mux_flash);
+
+    // Nonvolatile Storage
+    let virtual_flash_nv = static_init!(
+        capsules::virtual_flash::FlashUser<'static, sam4l::flashcalw::FLASHCALW>,
+        capsules::virtual_flash::FlashUser::new(mux_flash));
+    pub static mut NV_PAGEBUFFER: sam4l::flashcalw::Sam4lPage = sam4l::flashcalw::Sam4lPage::new();
+
+    let nv_nv_to_page = static_init!(
+        capsules::nonvolatile_to_pages::NonvolatileToPages<'static,
+            capsules::virtual_flash::FlashUser<'static, sam4l::flashcalw::FLASHCALW>>,
+        capsules::nonvolatile_to_pages::NonvolatileToPages::new(
+            virtual_flash_nv,
+            &mut NV_PAGEBUFFER));
+    hil::flash::HasClient::set_client(virtual_flash_nv, nv_nv_to_page);
+
+    pub static mut NV_BUFFER: [u8; 512] = [0; 512];
+    let nv = static_init!(
+        capsules::nonvolatile_storage_driver::NonvolatileStorage<'static>,
+        capsules::nonvolatile_storage_driver::NonvolatileStorage::new(
+            nv_nv_to_page, kernel::Grant::create(),
+            0x60000, // Start address for userspace accessible region
+            0x20000, // Length of userspace accessible region
+            0,       // Start address of kernel accessible region
+            0,       // Length of kernel accessible region
+            &mut NV_BUFFER));
+    hil::nonvolatile_storage::NonvolatileStorage::set_client(nv_nv_to_page, nv);
+
     let hail = Hail {
         console: console,
         gpio: gpio,
@@ -460,6 +495,7 @@ pub unsafe fn reset_handler() {
         ipc: kernel::ipc::IPC::new(),
         crc: crc,
         dac: dac,
+        nv: nv,
     };

     // Need to reset the nRF on boot
```