/**
 * @Author Vincent Johnson && Joshua French
 * Program to analyze:
 *  -Distance to an object
 *  -Angular Location of the object
 *  -Radial size of the object (the number within a sweep in which the object apperears)
 *  -Linear width (the actual width of the object)
 *
 */

double object_Distance(float *data[][]) {

    double ir_boundary = 80.0; //cm
    double sonar_boundary = 185.0; //cm

    //Find objects using sonar, then compare with IR

    //Once an object is found, use IR to find the angle (angular location)

    //Then find the radial size of the object

    //Then the linear width of the object, formula

    int i = 0;
    double objectsFound = 0.0;


//    for (; i <= 180; i++) {
//        if ((*(*data+i) + 1) < 185.0 && (*(*data+i)+0) < 80.0) {
//            objectsFound++;
//        }
//    }

    return objectsFound;


}
