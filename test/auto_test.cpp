#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <boost/test/unit_test.hpp>
#include "xtensor/xarray.hpp"
#include "xtensor/xeval.hpp"
#include "xtensor/xtensor.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xindex_view.hpp"
#include "xtensor/xmath.hpp"

#include "../lib/json/json.h"
#include "../src/technical_indicator/accumulation_distribution.h"
#include "../src/technical_indicator/momentum.h"
#include "../src/technical_indicator/simple_moving_average.h"
#include "../src/technical_indicator/ichimoku_cloud.h"
#include "../src/technical_indicator/standard_deviation.h"
#include "../src/technical_indicator/standard_variance.h"
#include "../src/technical_indicator/weighted_moving_average.h"
#include "../src/technical_indicator/typical_price.h"
#include "../src/technical_indicator/money_flow.h"
#include "../src/technical_indicator/moving_average_envelope.h"
#include "../src/technical_indicator/aroon.h"
#include "../src/technical_indicator/true_range.h"
#include "../src/technical_indicator/average_true_range.h"
#include "../src/technical_indicator/average_true_range_percent.h"


BOOST_AUTO_TEST_SUITE(extension_switch_test_suite)

namespace ti = signaldata::technical_indicator;

xt::xarray<int> arr {1, 2, 3, 4, 5, 6, 7, 8, 9};
xt::xtensor<int, 1> sample_volume({1519202, 1935211, 2105682, 3287669, 1366749,
    2537978, 1542199, 1868252, 1104452, 1332885, 1061692, 1166605, 1336585,
    1347697, 1029834, 1038696, 1047872, 1006612, 1250938, 1818671, 1383119,
    1010333, 1346189, 1386681, 1224889, 1261062, 1461210, 1005084, 1357185,
    1363345, 1295708, 1415128, 1194238, 1302225, 1666605, 1350875, 1528095,
    1689179, 2251047, 2020180, 3516933, 3752497, 3493251, 1662148, 1688375,
    2457377, 1306183, 1070454, 1027698, 1440905, 1290182, 1350308, 1325394,
    1197442, 1408924, 2017097, 1340535, 1515339, 1959033, 1735879, 1057392,
    1214756, 975962, 765537, 1132119, 1459628, 1271883, 1263581, 2598866,
    1769719, 1799654, 2128210, 1628602, 1904463, 1612531, 2029415, 1734099,
    1688473, 1718846, 2867074, 2279054, 1561981, 2575432, 613549, 1312981,
    1394174, 1630835, 1806264, 1533679, 1798360, 2943889, 3688274, 3592641,
    5909609, 3098510, 1769069, 1992570, 1970603, 2175216, 2350736, 2355890,
    2242679, 4354884, 2973486, 1794868, 1890712, 1447616, 1615814, 1393870,
    1500142, 2289265, 1056367, 1111934, 1368981, 907876, 1721575, 1496115,
    1163899, 1099909, 1213820, 1258706, 1489212, 1660201, 1349974, 1470280,
    1367271, 1472732});

xt::xtensor<double, 1> sample_close_data ({792.45, 802.88, 804.57, 809.93, 807.8,
    809.68, 812.2, 815.2, 812.5, 809.84, 815.65, 817.89, 815.34, 807.9,
    809.45, 795.17, 791.47, 776.18, 778.22, 764.46, 764.33, 775.88, 789.44,
    785.79, 780.2, 779, 785, 784.8, 775.97, 786.16, 779.98, 775.16, 753.22,
    771.75, 780.29, 805.59, 811.98, 802.03, 781.1, 782.19, 788.42, 805.48,
    809.9, 819.56, 817.35, 822.1, 828.55, 835.74, 824.06, 821.63, 827.09,
    821.49, 806.84, 804.6, 804.08, 811.77, 809.57, 814.17, 800.71, 803.08,
    801.23, 802.79, 800.38, 804.06, 802.64, 810.06, 810.73, 802.65, 814.96,
    815.95, 805.03, 799.78, 795.39, 797.97, 801.23, 790.46, 788.72, 798.82,
    788.48, 802.84, 807.99, 808.02, 796.87, 791.4, 789.85, 791.92, 795.82,
    793.22, 791.3, 793.6, 796.59, 796.95, 799.65, 802.75, 805.42, 801.19,
    805.96, 807.05, 808.2, 808.49, 807.48, 805.23, 806.93, 797.25, 798.92,
    800.12, 800.94, 791.34, 765.84, 761.97, 757.65, 757.52, 759.28, 754.41,
    757.08, 753.41, 753.2, 735.63, 735.8, 729.48, 732.51, 727.2, 717.78,
    707.26, 708.97, 704.89, 710.25});

xt::xtensor<double, 1> sample_open_data ({792.65, 802.44, 804.97, 810.1, 809.36, 809.74,
    813.47, 817.09, 813.84, 808.33, 816.82, 818.1, 814.88, 808.54, 809.14,
    793.75, 792.27, 777.49, 776.23, 765.78, 764.03, 777.64, 789.16, 785.8,
    779.42, 777.89, 784.3, 784.15, 777.1, 784.32, 780.36, 773.55, 751.27,
    773.14, 780.74, 806.06, 811.36, 802.77, 780.58, 783.8, 789.48, 803.57,
    810.31, 819.63, 817.57, 822.32, 827.92, 836.26, 825.95, 820.36, 827.71,
    822.4, 805.46, 804.97, 805.58, 811.53, 809.3, 812.61, 800.13, 804.02,
    801.56, 802.09, 801.82, 803.7, 803.34, 811.66, 811.08, 804.18, 816.33,
    816.32, 803.74, 799.0, 794.39, 797.65, 801.18, 788.73, 788.24, 799.76,
    787.64, 804.34, 807.17, 807.71, 795.59, 793.3, 789.95, 790.57, 796.19,
    793.37, 791.55, 794.59, 795.42, 796.95, 799.32, 802.44, 806.66, 799.92,
    805.57, 806.8, 809.1, 807.58, 806.91, 806.56, 806.8, 798.0, 797.97,
    798.52, 801.33, 791.09, 764.68, 761.23, 757.27, 756.87, 758.1, 753.83,
    755.59, 752.34, 753.65, 737.24, 735.76, 730.05, 732.79, 728.37, 717.19,
    707.42, 708.66, 703.46, 709.32});

xt::xtensor<double, 1> sample_high_data ({794.91, 805.3, 807.25, 812.88, 810.12, 812.05,
    814.66, 817.28, 814.84, 812.23, 818.64, 820.57, 818.29, 809.92, 812.34,
    797.73, 794.17, 778.71, 781.01, 767.06, 766.79, 778.32, 792.06, 787.82,
    782.73, 781.69, 787.13, 787.34, 778.89, 788.7, 782.51, 777.94, 755.72,
    774.1, 782.54, 808.14, 814.27, 804.36, 783.85, 784.19, 790.85, 808.37,
    812.32, 821.94, 819.38, 824.33, 831.01, 837.91, 826.29, 824.61, 829.38,
    823.9, 809.73, 806.96, 806.73, 814.6, 811.89, 816.2, 803.4, 805.91,
    803.84, 805.49, 802.58, 806.15, 805.4, 812.68, 812.88, 805.16, 817.83,
    818.51, 807.5, 802.37, 797.86, 800.07, 803.67, 793.35, 791.6, 801.07,
    790.55, 805.22, 810.23, 810.59, 798.98, 793.47, 792.58, 794.07, 798.18,
    795.72, 793.78, 795.88, 798.6, 799.11, 801.99, 805.62, 808.3, 803.94,
    808.05, 809.17, 810.42, 811.04, 809.84, 807.83, 809.04, 799.62, 801.36,
    802.27, 803.5, 793.89, 768.29, 764.1, 760.17, 760.06, 761.37, 756.72,
    759.35, 756.19, 755.8, 738.45, 738.62, 732.28, 734.98, 729.78, 719.97,
    709.99, 711.9, 707.4, 713.1});


xt::xtensor<double, 1> sample_low_data ({789.93, 800.42, 802.02, 807.1, 804.93, 807.34,
    810.0, 812.69, 810.45, 807.59, 813.25, 815.38, 812.94, 805.34, 807.21,
    792.78, 788.57, 773.93, 776.03, 762.46, 761.82, 773.87, 786.76, 783.52,
    778.02, 776.7, 782.61, 782.4, 773.05, 783.8, 777.94, 772.5, 750.74,
    769.26, 777.73, 803.46, 809.37, 799.27, 778.14, 779.91, 786.17, 803.02,
    807.05, 817.19, 814.46, 819.59, 825.75, 832.86, 821.76, 819.23, 824.87,
    819.47, 803.97, 802.26, 801.18, 809.5, 807.38, 811.34, 798.17, 800.12,
    799.14, 799.87, 797.48, 801.69, 799.9, 807.34, 807.84, 800.27, 812.84,
    813.54, 802.7, 797.21, 793.24, 795.63, 798.42, 788.0, 786.02, 795.97,
    785.85, 800.06, 805.14, 805.42, 794.71, 788.66, 786.9, 789.34, 793.47,
    790.34, 788.86, 791.24, 793.62, 794.67, 796.65, 800.24, 802.65, 798.96,
    803.41, 804.48, 805.43, 806.46, 804.76, 802.65, 804.44, 794.63, 796.36,
    797.94, 798.2, 789.16, 763.28, 759.63, 754.81, 755.09, 756.97, 752.26,
    754.28, 750.9, 750.26, 732.9, 733.42, 727.07, 730.1, 724.76, 715.58,
    704.91, 706.26, 702.58, 707.78});


BOOST_AUTO_TEST_CASE(test_case_average_true_range_percent, *boost::unit_test::enabled()) {
    auto data = sample_close_data;
    auto period = 6;

    auto atrp = ti::average_true_range_percent(data, period);

    std::cout << "atrp: " << atrp << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_average_true_range, *boost::unit_test::disabled()) {
    auto data = sample_close_data;
    auto period = 6;

    auto atr = ti::average_true_range(data, period);

    std::cout << "atr: " << atr << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_true_range, *boost::unit_test::disabled()) {
    auto data = sample_close_data;
    auto period = 6;

    auto tr = ti::true_range(data, period);

    std::cout << "tr: " << tr << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_aroon_up, *boost::unit_test::disabled()) {
    auto data = sample_close_data;
    auto period = 25;

    auto au = ti::aroon_up(data, period);

    std::cout << "au: " << au << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_aroon_down, *boost::unit_test::disabled()) {
    auto data = sample_close_data;
    auto period = 25;

    auto ad = ti::aroon_down(data, period);

    std::cout << "ad: " << ad << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_moving_average_envelope_lower_band, *boost::unit_test::disabled()) {
    auto data = sample_close_data;
    auto period = 6;
    auto env_percentage = 0.1;

    auto maelb = ti::moving_average_envelope_lower_band(data, period, env_percentage);

    std::cout << "maelb: " << maelb << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_moving_average_envelope_upper_band, *boost::unit_test::disabled()) {
    auto data = sample_close_data;
    auto period = 6;
    auto env_percentage = 0.1;

    auto maeub = ti::moving_average_envelope_upper_band(data, period, env_percentage);

    std::cout << "maeub: " << maeub << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_moving_average_envelope_center_band, *boost::unit_test::disabled()) {
    auto data = sample_close_data;
    auto period = 6;

    auto maecb = ti::moving_average_envelope_center_band(data, period);

    std::cout << "maecb: " << maecb << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_weighted_moving_average, *boost::unit_test::disabled()) {
    auto data = sample_close_data;
    auto period = 6;

    auto wmas = ti::weighted_moving_average(data, period);

    std::cout << "wma[" << "]: " << wmas << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_money_flow, *boost::unit_test::disabled()) {
    auto volume = sample_volume;
    auto close_data = sample_close_data;
    auto high_data = sample_high_data;
    auto low_data = sample_low_data;

    auto mfs = ti::money_flow(volume, close_data, high_data, low_data);

    std::cout << "mfs: " << mfs << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_typical_price, *boost::unit_test::disabled()) {
    auto close_data = sample_close_data;
    auto high_data = sample_high_data;
    auto low_data = sample_low_data;

    auto tps = ti::typical_price(close_data, high_data, low_data);

    std::cout << "tps: " << tps << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_accumulation_distribution, *boost::unit_test::disabled()) {
    auto ad = ti::accumulation_distribution(sample_close_data, sample_high_data, sample_low_data, sample_volume);
    std::cout << "ad: " << ad << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_standard_variance, *boost::unit_test::disabled()) {
    auto data = sample_close_data;
    auto period = 6;

    auto svs = ti::standard_variance(data, period);

    std::cout << "svs: " << svs << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_standard_deviation, *boost::unit_test::disabled()) {
    auto data = sample_close_data;
    auto period = 6;

    auto sds = ti::standard_deviation(data, period);

    std::cout << "sds: " << sds << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_ichimoku_cloud_senkou_b, *boost::unit_test::disabled()) {
    auto data = sample_close_data;
    auto period = 52;

    auto senkou_b = ti::ichimoku_cloud_senkou_b(data, period);

    std::cout << "senkou_b: " << senkou_b << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_ichimoku_cloud_senkou_a, *boost::unit_test::disabled()) {
    auto data = sample_close_data;
    auto period = 26;

    auto senkou_a = ti::ichimoku_cloud_senkou_a(data, period);

    std::cout << "senkou_a: " << senkou_a << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_ichimoku_cloud_chiku_span, *boost::unit_test::disabled()) {
    auto data = sample_close_data;
    auto period = 26;

    auto chiku_span = ti::ichimoku_cloud_chiku_span(data, period);

    std::cout << "chiku_span: " << chiku_span << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_ichimoku_cloud_kijunsen, *boost::unit_test::disabled()) {
    auto data = sample_close_data;
    auto period = 26;

    auto kijunsen = ti::ichimoku_cloud_kijunsen(data, period);

    std::cout << "kijunsen: " << kijunsen << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_ichimoku_cloud_tenkansen, *boost::unit_test::disabled()) {
    auto data = sample_close_data;
    auto period = 9;

    auto tenkansen = ti::ichimoku_cloud_tenkansen(data, period);

    std::cout << "tenkansen: " << tenkansen << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_momentum, *boost::unit_test::disabled()) {
    auto data = sample_close_data;
    auto period = 6;
    auto moms = ti::momentum(data, period);

    for (auto i = 0; i < moms.size(); i++)
        std::cout << "momentum: " << moms[i] << std::endl;
}

BOOST_AUTO_TEST_CASE(test_case_simple_moving_average, *boost::unit_test::disabled()) {
    auto data = sample_close_data;
    auto period = 6;
    auto smas = ti::simple_moving_average(sample_close_data, period);

    std::cout << "smas: " << smas << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()
