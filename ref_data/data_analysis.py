import json

with open('xy_8_inches.json') as json_file:
    data = json.load(json_file)
    step_size = data['stepsize']
    for x in range(-2, 3):
        for y in range(-2, 3):
            x_coord = -x * step_size
            y_coord = -y * step_size
            samples_list = data[str(x_coord) + ' ' + str(y_coord)]
            
            print(samples_list[0])
print('done!')