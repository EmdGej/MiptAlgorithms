Идея решения в следующем: храним два числа: максимальную высоту слева от левого указателя и максимальную высоту справа от правого указателя. Каждый раз вычисляем минимум из этих двух значений и работаем с соответствующим указателем: вода, которая может помещена в текущую позицию имеет объем V = max_left (max_right) - cur_height -> смещаем соответсвующий указатель и обновляем соответсвующую высоту