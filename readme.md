POSIX 线程
        >数据类型
                pthread_t：线程ID
                pthread_attr_t：线程属性
        >操纵线程函数
                pthread_create()创建一个线程
                pthread_exit()终止当前线程
                pthread_cancel()中断另外一个线程的运行
                pthread_join()阻塞当前的线程，直到另外一个线程运行结束
                pthread_attr_init()初始化线程的属性
                pthread_attr_setdetachstate()设置脱离状态的属性（决定这
                个线程在终止时是否可以被结合）
                pthread_attr_getdetachstate()获取脱离状态的属性
                pthread_attr_destroy()删除线程的属性
                pthread_kill()向线程发送一个信号
        >同步函数（用于 mutex 和条件变量）
                pthread_mutex_init() 初始化互斥锁
                pthread_mutex_destroy() 删除互斥锁
                pthread_mutex_lock()占有互斥锁（阻塞操作）
                pthread_mutex_trylock()试图占有互斥锁（不阻塞操作）。即，当互斥锁空闲时，将占有该锁；否则，立即返回。
                pthread_mutex_unlock() 释放互斥锁
                pthread_cond_init()初始化条件变量
                pthread_cond_destroy()销毁条件变量
                pthread_cond_signal() 唤醒第一个调用pthread_cond_wait():等待条件变量的特殊条件发生而进入睡眠的线程。
                Thread-local storage（或者以Pthreads术语，称作线程特有数据）：
                pthread_key_create(): 分配用于标识进程中线程特定数据的键
                pthread_setspecific(): 为指定线程特定数据键设置线程特定绑定
                pthread_getspecific(): 获取调用线程的键绑定，并将该绑定存储在 value 指向的位置中
                pthread_key_delete(): 销毁现有线程特定数据键
                pthread_attr_getschedparam();获取线程优先级
                pthread_attr_setschedparam();设置线程优先级
        >工具函数
                pthread_equal(): 对两个线程的线程标识号进行比较
                pthread_detach(): 分离线程
                pthread_self(): 查询线程自身线程标识号

          POSIX要求实现POSIX的系统为每个进程维护一个称之为 Key 的结构数组，这个数组中的每一个结构称之为一个线程特定数据元素。POSIX 规定系统实现的 Key 结构数组必须包含不少于 128 个线程特定数据元素，而每个线程特定数据元素中至少包含两项内容：使用标志和析构函数指针。线程特定数据元素中的使用标志指示这个数组元素是否正在使用，初始值为“不在使用”，我们稍后讨论线程特定数据元素中的析构函数指针。在后面的介绍中，我们假设Key 结构数组中包含 128 个元素。Key 结 构 数 组 中 每 个 元 素 的 索 引 (0~127） 称 之 为 键 (key) 当 一 个 线 程 调 用，pthread_key_create 创建一个新的线程特定数据元素时，系统搜索其所在进程的 Key 结构数组，找出其中第一个不在使用的元素，并返回该元素的键。这个函数的形式为：

                                int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));

          参数 key 为一个 pthread_key_t变量的指针，用于保存得到的键值。参数 destructor为指定的析构函数的指针。除了 Key 结构数组，系统还在进程中维护关于每个线程的多种信息。这些特定于线程的信息被保存于称之为 Pthread 的结构中。Pthread 结构中包含名为 pkey 的指针数组，其长度为128，初始值为空。这 128 个指针与 Key 结构数组的 128 个线程特定数据元素一一对应。在调用 pthread_key_create 得到一个键之后，每个线程可以依据这个键操作自己的 pkey 指针数组中对应的指针，这通过 pthread_getspecific 和 pthread_setspecific 函数来实现。这两个函数的形式为：

                                void *pthread_getspecific(pthread_key_t key);
                                int pthread_setspecific(pthread_key_t key, const void *value);

              pthread_getspecific 返回 pkey 中对应于 key 的指针，而 pthread_setspecific 将 pkey 中对应于 key 的指针设置为 value。我们使用线程特定数据机制，就是要使线程中的函数可以共享一些数据。如果我们在线程中通过 malloc 获得一块内存，并把这块内存的指针通过 pthread_setspecific 设置到 pkey指针数组中对应于 key 的位置，那么线程中调用的函数即可通过 pthread_getspecific 获得这个指针，这就实现了线程内部数据在各个函数间的共享。当一个线程终止时，系统将扫描该线程的 pkey数组，为每个非空的 pkey指针调用相应的析构函数，因此只要将执行回收的函数的指针在调用 pthread_key_create 时作为函数的参数，即可在线程终止时自动回收分配的内存区。

