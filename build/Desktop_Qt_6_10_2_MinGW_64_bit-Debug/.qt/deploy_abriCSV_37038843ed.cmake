include("C:/Dev/Qt/abriCSV/build/Desktop_Qt_6_10_2_MinGW_64_bit-Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/abriCSV-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "C:/Dev/Qt/abriCSV/build/Desktop_Qt_6_10_2_MinGW_64_bit-Debug/abriCSV.exe"
    GENERATE_QT_CONF
)
