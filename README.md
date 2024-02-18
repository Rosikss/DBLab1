## Умова:
##### Опція у програмі: 1, 6:
1. функція читання заданого запису та заданих підзаписів на основі прямого
індексного доступу; (get-m, get-s) 
##### Опція у програмі: 3, 8:
2. функція вилучення заданого запису чи заданого підзапису; при вилученні запису
необхідно вилучати всі його підзаписи; (del-m, del-s)
##### Опція у програмі: 2, 7:
3. функція оновлення значення заданого поля заданого запису чи заданого підзапису;
(update-m, update-s)
##### Опція у програмі: 0, 5:
4. функція внесення запису та/чи підзаписів в файли (insert-m, insert-s).
##### Опція у програмі: 10:
5. функція підрахунку кількості записів, підзаписів загалом, а також кількості
підзаписів для кожного запису (calc-m, calc-s).
##### Опція у програмі: 4, 9:
6. утиліти (ut-m, ut-s) читання та друку всіх полів (включаючи службові) master-file та
slave-file.

## <span style="color:red">Перед запуском:</span>
Переконайтеся, що файли `storage_garbage.txt` та `ingredient_garbage.txt` завжди мають 0 на першому рядку. Крім того, файли `ingredient.fl`, `storage.fl`, `storage.ind` повинні бути пустими.

## <span style="color:red">Запуск:</span>
Не натискайте 4 на старті програми, перед цим введіть хоча б 1 мастер запис.

## Тестувалося на:
1. Введення 5 master-записів.
2. Для 3-х master-записів введено 1, 2 та їх підлеглі записи.
3. Тестування на ut-m, ut-s.
4. Вилучення master-запису з двома підлеглими.
5. Вилучення підлеглого запису від master-запису з 3-ма підлеглими.
6. Тестування на ut-m, ut-s.
7. Введення ще одного master-запису та підлеглого до нього запису.
8. Тестування на ut-m, ut-s.
9. Оновлення поля master-запису та поля підлеглого запису.
10. Тестування на ut-m, ut-s.


