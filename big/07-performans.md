# Performans

Üç sistem, üç ayrı yer. Biri diğerinin yerine geçmez.

## 1. Yüz eleme

Çizilen üçgen sayısı. `03-dunya.md`. Chunk başına bir draw.

## 2. İş parçacığı

Donma, mesh ve arazi ana karede hesaplanınca olur. GPU yüklemesi tersine ana karede olmak zorunda.

```text
ana thread                         işçiler (2–4)
-----------                        -------------
oyuncu hareketi
eksik chunk'ı kuyruğa yaz    -->   üret veya diskten oku
                                   ışık
                                   iki mesh vektörü doldur
hazır vektörü GPU'ya al      <--   state = ReadyToUpload
VAO/VBO, state = Uploaded
çiz
```

Kurallar:

- `blocks` yazılırken o chunk meshlenmez. `dirty` atomik veya mutex. `SetBlock` ana thread'de, mesh başlamadan kilit.
- Worker `gl*` çağırmaz.
- `std::thread(...).detach()` ile sınırsız thread açılmaz. Kuyruk + sabit havuz.
- Bir karede GPU'ya alınan mesh sayısı sınırlı (örneğin 4). Hepsini bir karede basmak yine spike yapar.
- Chunk silinmeden kayıt (`08`). Silinen chunk'ın VAO'su ana thread'de `glDelete`.

Durum:

| state | anlam |
|---|---|
| Empty | ayrıldı, veri yok |
| Generating | işçi içinde |
| ReadyToUpload | vektör dolu, GL yok |
| Uploaded | çizilebilir |

`dirty` olursa Uploaded iken yeniden Generating'e alınır. Eski VAO, yeni veri `glBufferData` ile gelene kadar çizilmeye devam eder (son iyi mesh). Bir kareli delik olmasın.

## 3. Frustum

Sohbette iki kez anlatıldı. Uygulama tek.

`Frustum` altı düzlem. Her kare `projection * view` üzerinden Gribb–Hartmann. Düzlem normalize.

Chunk AABB'si altı düzlemin de içinde değilse o chunk'ın `glDraw*` çağrısı yok. Test CPU'da, ucuz. 1000 yüklü chunk'ın ~100–150'si çizilir.

Saydam mesh aynı testten geçer; ayrı frustum yok.

Gölge geçişi bu frustum'u kullanmaz (`06`).

Görüş mesafesi ayarı hem yükleme yarıçapıdır hem projeksiyonun `far` değeriyle uyumlu olmalıdır. Far, yarıçaptan kısaysa frustum zaten keser ama pop-in kötü olur. Far'ı chunk yarıçapının biraz üstünde tut.

## Bilerek yapılmayanlar

- Greedy meshing, occlusion culling (mağara ağzı), LOD. Ölçmeden eklenmez.
- Her bloğu ayrı draw, şeffaf sıralamayı her kare dünya genelinde kusursuz çözmek. Chunk sırası yeter, su kenarında nadir hata kabul.
