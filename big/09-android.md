# Android

Masaüstü oynanmadan bu mile geçilmez (`11`). Çekirdek aynı kaynak. Ayrılan yalnız pencere bağlamı, girdi ve dosya.

## Grafik

- Masaüstü: OpenGL 3.3 Core, `#version 330 core`
- Android: OpenGL ES 3.0, `#version 300 es` ve fragmanda `precision mediump float;`

Shader çiftini iki kez yazmamak için başa küçük bir önek koyan yükleyici yeter. Gömülü fonksiyonlar (tam sayı doku, compute) kullanma; ES'te yok.

CMake:

```cmake
if(ANDROID)
  target_link_libraries(VoxelEngine PRIVATE GLESv3 EGL android log)
else()
  find_package(OpenGL REQUIRED)
  target_link_libraries(VoxelEngine PRIVATE OpenGL::GL)
endif()
find_package(SDL2 REQUIRED)
target_link_libraries(VoxelEngine PRIVATE SDL2::SDL2)
```

SDL2 Android'de pencereyi ve GL bağlamını kurar. Ayrı Java GL yüzeyi yazılmaz. APK'yı SDL'nin Android projesi + NDK üretir; oyun kodu yine `src/`.

## Dosya

APK içinde `fopen("assets/...")` çalışmaz. Tek kapı `02`'deki `ReadFile`. Android kolu:

```cpp
SDL_RWops* file = SDL_RWFromFile(path, "rb");
```

SDL, Java'dan `AAssetManager` taşımadan APK içini okur. Dönüş `vector<unsigned char>`. PNG → `stbi_load_from_memory`. Ses → `ma_decoder_init_memory`. Shader → baytların string hali.

Kayıt (`saves/`, `settings.json`) APK'ya yazılmaz. `SDL_AndroidGetInternalStoragePath()` altına. Bu ayrım `ReadFile` (paket, salt okunur) ile `SavePath` (yazılır) olarak iki fonksiyon. Karıştırılırsa ayar kaydı telefonda sessizce ölür.

## Dokunmatik

Parmak kimliği (finger id) şart, tek "mouse" yetmez.

- Sol alt: hareket joystick'i. Merkez parmak inince sabitlenir, knob parmağı takip eder, sapma WASD vektörü olur. Bırakınca sıfır.
- Sağ alt: bakış alanı. Delta, fare hassasiyetiyle yaw/pitch.
- Sağ üst iki düğme: zıpla, kırma/koyma ya da sağ-sol eylem. v1: bir düğme kırma (basılı = madencilik), bir düğme koyma.
- Çift dokunuş veya ayrı düğme: envanter. Üretim masası zaten dünyada sağ eylem.

Joystick HUD'da iki yuvarlak (zemin + knob), alfa düşük, `ui` shader. Parmak koordinatı piksel, ortho ile aynı uzay.

Masaüstünde bu kontrol çizilmez. `#if` yerine `App::isTouch` çalışma anı bayrağı; aynı binary'de klavye de durur (Bluetooth klavye).

## Bilerek yok

- Mağaza, reklam, hesap.
- Vulkan.
- Dokunmatikte sağ-sol tuş ayrımı için klavye kısayolu şartı. Düğmeler yeter.
