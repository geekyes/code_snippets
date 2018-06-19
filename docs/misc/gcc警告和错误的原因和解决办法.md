1. Type passing discards 'volatile' qualifier from pointer target type
    大概的意思：丢弃(discards) 'volatile' 这个限定词。
    认为跟强制类型转换一样的，如uint16_t隐式转换成uint8_t，会得到警告“丢失精度”

    解决办法：强制转换到需要的类型(相当于显性转换，明确指出丢弃 'volatile')
    后果：编译器会优化代码
