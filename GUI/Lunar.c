#include "Lunar.h"

/* Tháng âm lịch (index 1–12; index 0 - placeholder) {"----", "T Giêng", "T2", "T3", "T4", "T5", "T6",
                                       "T7", "T8", "T9", "T10", "T11", "T Chạp"} */
const char Lunar_MonthString[13][7] = {"----", "Th1", "Th2", "Th3", "Th4", "Th5", "Th6",
                                       "Th7", "Th8", "Th9", "Th10", "Th11", "Th12"};

/* Tiền tố tháng nhuận: index 0 = bthg, index 1 = Nhuận */
const char Lunar_MonthLeapString[2][4] = {" ", "闰"};

/* Ngày âm lịch (index 1–30; index 0 - placeholder) */
const char Lunar_DateString[31][7] = {"----", "M1", "M2", "M3", "M4", "M5", "M6", "M7",
                                      "M8", "M9", "M10", "11", "12", "13", "14", "15",
                                      "16", "17", "18", "19", "20", "21", "22", "23",
                                      "24", "25", "26", "27", "28", "29", "30"};

/* Ngày trong tuần: Sunday(日), Monday(一) … Saturday(六) {"CN", "T2", "T3", "T4", "T5", "T6", "T7"} */
const char Lunar_DayString[7][4] = {"CN", "T2", "T3", "T4", "T5", "T6", "T7"};

/* 12 con giáp (year % 12) {"Thân", "Dậu", "Tuất", "Hợi", "Tý", "Sửu", "Dần", "Mão", "Thìn", "Tị", "Ngọ", "Mùi"} */
const char Lunar_ZodiacString[12][4] = {"猴", "鸡", "狗", "猪", "鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊"};

/* 10 thiên can (year % 10) {"Canh", "Tân", "Nhâm", "Quý", "Giáp", "Ất", "Bính", "Đinh", "Mậu", "Kỷ"} */
const char Lunar_StemStrig[10][4] = {"庚", "辛", "壬", "癸", "甲", "乙", "丙", "丁", "戊", "己"};

/* 12 địa chi, tương tự con giáp, chủ yếu sử dụng để tính toán {"Thân", "Dậu", "Tuất", "Hợi", "Tý", "Sửu", "Dần", "Mão", "Thìn", "Tị", "Ngọ", "Mùi"} */
const char Lunar_BranchStrig[12][4] = {"申", "酉", "戌", "亥", "子", "丑", "寅", "卯", "辰", "巳", "午", "未"};



/*
 * Bit-Packed dữ liệu ngày-tháng âm lịch cho các năm 2000–2053 (starting offset stored in element [0]).
 *
 * Lưu 32 bit, bao gồm:
 *   bits [16:13] – index tháng nhuận (0 = bthg, 1= Nhuận)
 *   bits [12: 0] – mỗi bit tương ứng 1 tháng âm lịch (tối đa 13 tháng - năm nhuận):
 *                  bit = 1 → tháng 30 ngày, bit = 0 → tháng 29 ngày
 *
 * Dữ liệu năm 2054–2199 được comment để tiết kiệm flash.
 */
/* 2000 ~ 2199  */
const uint32_t lunar_month_days[] = {
    1997,
    0x0000B26D, 0x0000125C, 0x0000192C, 0x00009A95, 0x00001A94, 0x00001B4A, 0x00004B55, 0x00000AD4, 0x0000F55B,
    0x000004BA, 0x0000125A, 0x0000B92B, 0x0000152A, 0x00001694, 0x000096AA, 0x000015AA, 0x00012AB5, 0x00000974,
    0x000014B6, 0x0000CA57, 0x00000A56, 0x00001526, 0x00008E95, 0x00000D54, 0x000015AA, 0x000049B5, 0x0000096C,
    0x0000D4AE, 0x0000149C, 0x00001A4C, 0x0000BD26, 0x00001AA6, 0x00000B54, 0x00006D6A, 0x000012DA, 0x0001695D,
    0x0000095A, 0x0000149A, 0x0000DA4B, 0x00001A4A, 0x00001AA4, 0x0000BB54, 0x000016B4, 0x00000ADA, 0x0000495B,
    0x00000936, 0x0000F497, 0x00001496, 0x0000154A, 0x0000B6A5, 0x00000DA4, 0x000015B4, 0x00006AB6, 0x0000126E,
    /*0x0001092F, 0x0000092E, 0x00000C96, 0x0000CD4A, 0x00001D4A, 0x00000D64, 0x0000956C, 0x0000155C, 0x0000125C,
    0x0000792E, 0x0000192C, 0x0000FA95, 0x00001A94, 0x00001B4A, 0x0000AB55, 0x00000AD4, 0x000014DA, 0x00008A5D,
    0x00000A5A, 0x0001152B, 0x0000152A, 0x00001694, 0x0000D6AA, 0x000015AA, 0x00000AB4, 0x000094BA, 0x000014B6,
    0x00000A56, 0x00007527, 0x00000D26, 0x0000EE53, 0x00000D54, 0x000015AA, 0x0000A9B5, 0x0000096C, 0x000014AE,
    0x00008A4E, 0x00001A4C, 0x00011D26, 0x00001AA4, 0x00001B54, 0x0000CD6A, 0x00000ADA, 0x0000095C, 0x0000949D,
    0x0000149A, 0x00001A2A, 0x00005B25, 0x00001AA4, 0x0000FB52, 0x000016B4, 0x00000ABA, 0x0000A95B, 0x00000936,
    0x00001496, 0x00009A4B, 0x0000154A, 0x000136A5, 0x00000DA4, 0x000015AC, 0x0000CAB6, 0x0000126E, 0x0000092E,
    0x00008C97, 0x00000A96, 0x00000D4A, 0x00006DA5, 0x00000D54, 0x0000F56A, 0x0000155A, 0x00000A5C, 0x0000B92E,
    0x0000152C, 0x00001A94, 0x00009D4A, 0x00001B2A, 0x00016B55, 0x00000AD4, 0x000014DA, 0x0000CA5D, 0x00000A5A,
    0x0000151A, 0x0000BA95, 0x00001654, 0x000016AA, 0x00004AD5, 0x00000AB4, 0x0000F4BA, 0x000014B6, 0x00000A56,
    0x0000B517, 0x00000D16, 0x00000E52, 0x000096AA, 0x00000D6A, 0x000165B5, 0x0000096C, 0x000014AE, 0x0000CA2E,
    0x00001A2C, 0x00001D16, 0x0000AD52, 0x00001B52, 0x00000B6A, 0x0000656D, 0x0000055C, 0x0000F45D, 0x0000145A,
    0x00001A2A, 0x0000DA95, 0x000016A4, 0x00001AD2, 0x00008B5A, 0x00000AB6, 0x0001455B, 0x000008B6, 0x00001456,
    0x0000D52B, 0x0000152A, 0x00001694, 0x0000B6AA, 0x000015AA, 0x00000AB6, 0x000064B7, 0x000008AE, 0x0000EC57,
    0x00000A56, 0x00000D2A, 0x0000CD95, 0x00000B54, 0x0000156A, 0x00008A6D, 0x0000095C, 0x000014AE, 0x00004A56,
    0x00001A54, 0x0000DD2A, 0x00001AAA, 0x00000B54, 0x0000B56A, 0x000014DA, 0x0000095C, 0x000074AB, 0x0000149A,
    0x0000FA4B, 0x00001652, 0x000016AA, 0x0000CAD5, 0x000005B4*/};


/*
 * Bit-Packed dữ liệu ngày mùng 1 tháng Giêng âm lịch theo lịch dương cho năm 2000–2053 (starting offset in element [0]).
 *
 * Lưu 32 bit, bao gồm:
 *   [20: 9] – Năm Dương lịch  (12 bits)
 *   [ 8: 5] – tháng           ( 4 bits)
 *   [ 4: 0] – ngày             ( 5 bits)
 *
 * Bảng này lưu ngày Dương lịch tương ứng với ngày mùng 1 tháng Giêng của mỗi năm âm lịch,
 * sử dụng làm mốc tính toán độ lệch ngày
 * 
 * Dữ liệu cho các năm 2054–2199 có sẵn nhưng comment lại.
 */
/* 2000 ~ 2199  */
const uint32_t solar_1_1[] = {
    1997,
    0x000F9C3C, 0x000F9E50, 0x000FA045, 0x000FA238, 0x000FA44C, 0x000FA641, 0x000FA836, 0x000FAA49, 0x000FAC3D,
    0x000FAE52, 0x000FB047, 0x000FB23A, 0x000FB44E, 0x000FB643, 0x000FB837, 0x000FBA4A, 0x000FBC3F, 0x000FBE53,
    0x000FC048, 0x000FC23C, 0x000FC450, 0x000FC645, 0x000FC839, 0x000FCA4C, 0x000FCC41, 0x000FCE36, 0x000FD04A,
    0x000FD23D, 0x000FD451, 0x000FD646, 0x000FD83A, 0x000FDA4D, 0x000FDC43, 0x000FDE37, 0x000FE04B, 0x000FE23F,
    0x000FE453, 0x000FE648, 0x000FE83C, 0x000FEA4F, 0x000FEC44, 0x000FEE38, 0x000FF04C, 0x000FF241, 0x000FF436,
    0x000FF64A, 0x000FF83E, 0x000FFA51, 0x000FFC46, 0x000FFE3A, 0x0010004E, 0x00100242, 0x00100437, 0x0010064B,
    /*0x00100841, 0x00100A53, 0x00100C48, 0x00100E3C, 0x0010104F, 0x00101244, 0x00101438, 0x0010164C, 0x00101842,
    0x00101A35, 0x00101C49, 0x00101E3D, 0x00102051, 0x00102245, 0x0010243A, 0x0010264E, 0x00102843, 0x00102A37,
    0x00102C4B, 0x00102E3F, 0x00103053, 0x00103247, 0x0010343B, 0x0010364F, 0x00103845, 0x00103A38, 0x00103C4C,
    0x00103E42, 0x00104036, 0x00104249, 0x0010443D, 0x00104651, 0x00104846, 0x00104A3A, 0x00104C4E, 0x00104E43,
    0x00105038, 0x0010524A, 0x0010543E, 0x00105652, 0x00105847, 0x00105A3B, 0x00105C4F, 0x00105E45, 0x00106039,
    0x0010624C, 0x00106441, 0x00106635, 0x00106849, 0x00106A3D, 0x00106C51, 0x00106E47, 0x0010703C, 0x0010724F,
    0x00107444, 0x00107638, 0x0010784C, 0x00107A3F, 0x00107C53, 0x00107E48, 0x0010803D, 0x00108250, 0x00108446,
    0x0010863A, 0x0010884E, 0x00108A42, 0x00108C36, 0x00108E4A, 0x0010903E, 0x00109251, 0x00109447, 0x0010963B,
    0x0010984F, 0x00109A43, 0x00109C37, 0x00109E4B, 0x0010A041, 0x0010A253, 0x0010A448, 0x0010A63D, 0x0010A851,
    0x0010AA45, 0x0010AC39, 0x0010AE4D, 0x0010B042, 0x0010B236, 0x0010B44A, 0x0010B63E, 0x0010B852, 0x0010BA47,
    0x0010BC3B, 0x0010BE4F, 0x0010C044, 0x0010C237, 0x0010C44B, 0x0010C641, 0x0010C854, 0x0010CA48, 0x0010CC3D,
    0x0010CE50, 0x0010D045, 0x0010D239, 0x0010D44C, 0x0010D642, 0x0010D837, 0x0010DA4A, 0x0010DC3E, 0x0010DE52,
    0x0010E047, 0x0010E23A, 0x0010E44E, 0x0010E643, 0x0010E838, 0x0010EA4B, 0x0010EC41, 0x0010EE54, 0x0010F049,
    0x0010F23C, 0x0010F450, 0x0010F645, 0x0010F839, 0x0010FA4C, 0x0010FC42, 0x0010FE37, 0x0011004B, 0x0011023E,
    0x00110452, 0x00110647, 0x0011083B, 0x00110A4E, 0x00110C43, 0x00110E38, 0x0011104C, 0x0011123F, 0x00111435,
    0x00111648, 0x0011183C, 0x00111A4F, 0x00111C45, 0x00111E39, 0x0011204D, 0x00112242, 0x00112436, 0x0011264A,
    0x0011283E, 0x00112A51, 0x00112C46, 0x00112E3B, 0x0011304F*/};


/**
 * @brief  Extract a bit-field from a 32-bit integer.
 *
 * Reads `length` consecutive bits starting at bit position `shift`
 * from the value `data` and returns them right-aligned (zero-extended).
 *
 * Example: GetBitInt(0xABCD, 4, 8) extracts bits [11:8] → returns 0xC.
 *
 * @param  data    Source 32-bit value.
 * @param  length  Number of bits to extract (1–32).
 * @param  shift   Least-significant bit position of the field.
 * @return         The extracted bit-field as an unsigned 32-bit integer.
 */
static uint32_t GetBitInt(uint32_t data, uint8_t length, uint8_t shift) {
    return (data & (((1 << length) - 1) << shift)) >> shift;
}


/**
 * @brief  Convert a Gregorian date to a scalar day-count (proleptic Gregorian).
 *
 * Uses the standard astronomical day-number formula.
 * WARNING: Dates before October 1582 are inaccurate (Gregorian reform not applied).
 *
 * The result is suitable only for computing differences between two dates
 * (the absolute origin is arbitrary).
 *
 * @param  y  Gregorian year  (e.g. 2024).
 * @param  m  Month (1–12).
 * @param  d  Day   (1–31).
 * @return    An integer representing the date as a monotonically increasing day count.
 */
// WARNING: Dates before Oct. 1582 are inaccurate
static uint16_t SolarToInt(uint16_t y, uint8_t m, uint8_t d) {
    m = (m + 9) % 12;
    y = y - m / 10;
    return 365 * y + y / 4 - y / 100 + y / 400 + (m * 306 + 5) / 10 + (d - 1);
}


/**
 * @brief  Chuyển đổi ngày Dương lịch sang Âm lịch
 *
 * Tra cứu ngày Dương lịch của Tết Nguyên Đán sử dụng bảng `solar_1_1[]`,
 * tính toán số ngày chênh lệch, so đỐ tra bảng `lunar_month_days[]`
 * để xác định ngày/tháng âm lịch, bao gồm cả tháng nhuận.
 *
 * Dữ liệu trả về: `*lunar`:
 *   - Year   : Năm âm lịch (trùng với năm Dương lịch trong phần lớn thời gian của năm).
 *   - Month  : Tháng âm lịch (1–12).
 *   - Date   : Ngày âm lịch trong tháng (1–30).
 *   - IsLeap : 1 nếu là tháng nhuận, 0 nếu là tháng thường.
 *
 * Nếu chuyển đổi thất bại (ngày nằm ngoài phạm vi hỗ trợ 2000–2053 hoặc
 * tháng/ngày không hợp lệ), cả bốn trường sẽ được đặt về 0.
 *
 * Phạm vi ngày Dương lịch được hỗ trợ: các năm có trong bảng `solar_1_1[]`
 * (hiện tại từ 2000–2053).
 *
 * @param[out] lunar        Pointer to a Lunar_Date structure to receive the result.
 * @param[in]  solar_year   Gregorian year  (e.g. 2024).
 * @param[in]  solar_month  Gregorian month (1–12).
 * @param[in]  solar_date   Gregorian day   (1–31).
 */
void LUNAR_SolarToLunar(struct Lunar_Date* lunar, uint16_t solar_year, uint8_t solar_month, uint8_t solar_date) {
    uint8_t i, lunarM, m, d, leap, dm;
    uint16_t year_index, lunarY, y, offset;
    uint32_t solar_data, solar11, days;

     /* Kiểm tra tính hợp lệ của dữ liệu đầu vào */
    if (solar_month < 1 || solar_month > 12 || solar_date < 1 || solar_date > 31 || (solar_year - solar_1_1[0] < 3) ||
        ((solar_year - solar_1_1[0]) > (sizeof(solar_1_1) / sizeof(uint32_t) - 2))) {
        lunar->Year = 0;
        lunar->Month = 0;
        lunar->Date = 0;
        lunar->IsLeap = 0;
        return;
    }

     /* Xác định năm âm lịch dựa vào ngày Dương lịch */
    year_index = solar_year - solar_1_1[0];
    solar_data = ((uint32_t)solar_year << 9) | ((uint32_t)solar_month << 5) | ((uint32_t)solar_date);
    if (solar_1_1[year_index] > solar_data) {
        year_index -= 1;
    }

    /* Tính toán ngày Dương lịch của mùng 1 tết năm hiện tại */
    solar11 = solar_1_1[year_index];
    y = GetBitInt(solar11, 12, 9);
    m = GetBitInt(solar11, 4, 5);
    d = GetBitInt(solar11, 5, 0);

    /* Tính toán số ngày từ ngày m1 Tết đến ngày âm lịch hiện tại */
    offset = SolarToInt(solar_year, solar_month, solar_date) - SolarToInt(y, m, d);

    /* Lấy thông tin tháng nhuận và số ngày mỗi tháng âm lịch */
    days = lunar_month_days[year_index];
    leap = GetBitInt(days, 4, 13); /* (0 = bthg, 1 = Nhuận) */

    lunarY = year_index + solar_1_1[0];
    lunarM = 1;
    offset += 1; /* Ngày trong tháng âm lịch (đánh số từ 1) */

    /* Duyệt tối đa 13 tháng âm lịch để xác định đúng tháng và ngày */
    for (i = 0; i < 13; i++) {
        /* Bit = 1 → tháng đủ (30 ngày), Bit = 0 → tháng thiếu (29 ngày) */
        if (GetBitInt(days, 1, 12 - i) == 1) {
            dm = 30;
        } else {
            dm = 29;
        }
        if (offset > dm) {
            lunarM += 1;
            offset -= dm;
        } else {
            break;
        }
    }

    /* Điều chỉnh nếu năm có tháng nhuận */
    lunar->IsLeap = 0;
    if (leap != 0 && lunarM > leap) {
        if (lunarM == leap + 1) {
            lunar->IsLeap = 1; /* Tháng hiện tại là tháng nhuận */
        }
        lunarM -= 1;
    }
    lunar->Month = lunarM;
    lunar->Date = offset;
    lunar->Year = lunarY;
}

/**
 * @brief  Trả về con giáp của năm Âm lịch hiện tại
 *
 * Chu kỳ 12 con giáp được xác định theo phép lấy dư năm % 12:
 *   0=Thân, 1=Dậu, 2=Tuất, 3=Hợi, 4=Tý, 5=Sửu,
 *   6=Dần, 7=Mão, 8=Thìn, 9=Tỵ, 10=Ngọ, 11=Mùi.
 *
 * Chỉ số trả về được dùng để tra cứu trong bảng Lunar_ZodiacString[].
 *
 * @param  lunar  Pointer to a populated Lunar_Date structure.
 * @return        Zodiac index in the range [0, 11].
 */
uint8_t LUNAR_GetZodiac(const struct Lunar_Date* lunar) { return lunar->Year % 12; }

/**
 * @brief  Trả về chỉ số Thiên Can của một năm âm lịch.
 *
 * Chu kỳ 10 Thiên Can được xác định theo phép lấy dư năm % 10.
 * Chỉ số trả về được dùng để tra cứu trong bảng Lunar_StemString[].
 *
 * @param  lunar  Pointer to a populated Lunar_Date structure.
 * @return        Stem index in the range [0, 9].
 */
uint8_t LUNAR_GetStem(const struct Lunar_Date* lunar) { return lunar->Year % 10; }


/**
 * @brief  Trả về chỉ số Địa Chi của một năm âm lịch.
 *
 * Chu kỳ 12 Địa Chi được xác định theo phép lấy dư năm % 12.
 * Chỉ số trả về được dùng để tra cứu trong bảng Lunar_BranchString[].
 *
 * Lưu ý: Chu kỳ Địa Chi trùng với chu kỳ 12 con giáp, do đó
 * hàm này trả về cùng một giá trị như LUNAR_GetZodiac().
 *
 * @param  lunar  Pointer to a populated Lunar_Date structure.
 * @return        Branch index in the range [0, 11].
 */
uint8_t LUNAR_GetBranch(const struct Lunar_Date* lunar) { return lunar->Year % 12; }

/*********************************************************************************************************
 **         Tính toán 12 tiết khí (Solar terms)
 **------------------------------------------------------------------------------------------------------
 ********************************************************************************************************/

/*
 * Bảng tiết khí hàng năm cho các năm 2000–2050.
 *
 * Mỗi năm được lưu 3 bytes (24 bits, mỗi bit 1 tiết khí).
 * Bit có giá trị 1 cho biết ngày của tiết khí khác với ngày cơ sở trong
 * bảng `days[]` một ngày (±1 ngày). Hướng điều chỉnh (+1 hay -1) phụ thuộc
 * vào từng tiết khí và từng năm (xem hàm GetJieQi() để biết chi tiết).
 *
 * Có thể mở rộng bảng này cho các năm tiếp theo bằng cách tuân theo cùng
 * nguyên tắc encoding.
 */
static const uint8_t YearMonthBit[160] = {
    0x4E, 0xA6, 0x99,  // 2000
    0x9C, 0xA2, 0x98,  // 2001
    0x80, 0x00, 0x18,  // 2002
    0x00, 0x10, 0x24,  // 2003
    0x4E, 0xA6, 0x99,  // 2004
    0x9C, 0xA2, 0x98,  // 2005
    0x80, 0x82, 0x18,  // 2006
    0x00, 0x10, 0x24,  // 2007
    0x4E, 0xA6, 0xD9,  // 2008
    0x9E, 0xA2, 0x98,  // 2009

    0x80, 0x82, 0x18,  // 2010
    0x00, 0x10, 0x04,  // 2011
    0x4E, 0xE6, 0xD9,  // 2012
    0x9E, 0xA6, 0xA8,  // 2013
    0x80, 0x82, 0x18,  // 2014
    0x00, 0x10, 0x00,  // 2015
    0x0F, 0xE6, 0xD9,  // 2016
    0xBE, 0xA6, 0x98,  // 2017
    0x88, 0x82, 0x18,  // 2018
    0x80, 0x00, 0x00,  // 2019

    0x0F, 0xEF, 0xD9,  // 2020
    0xBE, 0xA6, 0x99,  // 2021
    0x8C, 0x82, 0x98,  // 2022
    0x80, 0x00, 0x00,  // 2023
    0x0F, 0xEF, 0xDB,  // 2024
    0xBE, 0xA6, 0x99,  // 2025
    0x9C, 0xA2, 0x98,  // 2026
    0x80, 0x00, 0x18,  // 2027
    0x0F, 0xEF, 0xDB,  // 2028
    0xBE, 0xA6, 0x99,  // 2029

    0x9C, 0xA2, 0x98,  // 2030
    0x80, 0x00, 0x18,  // 2031
    0x0F, 0xEF, 0xDB,  // 2032
    0xBE, 0xA2, 0x99,  // 2033
    0x8C, 0xA0, 0x98,  // 2034
    0x80, 0x82, 0x18,  // 2035
    0x0B, 0xEF, 0xDB,  // 2036
    0xBE, 0xA6, 0x99,  // 2037
    0x8C, 0xA2, 0x98,  // 2038
    0x80, 0x82, 0x18,  // 2039

    0x0F, 0xEF, 0xDB,  // 2040
    0xBE, 0xE6, 0xD9,  // 2041
    0x9E, 0xA2, 0x98,  // 2042
    0x80, 0x82, 0x18,  // 2043
    0x0F, 0xEF, 0xFB,  // 2044
    0xBF, 0xE6, 0xD9,  // 2045
    0x9E, 0xA6, 0x98,  // 2046
    0x80, 0x82, 0x18,  // 2047
    0x0F, 0xFF, 0xFF,  // 2048
    0xFC, 0xEF, 0xD9,  // 2049
    0xBE, 0xA6, 0x18,  // 2050
};

/*
 * Ngày cơ sở (ngày trong tháng) của 24 tiết khí, mỗi tháng có 2 tiết khí.
 *
 * Các cặp chỉ số:
 *   [0,1]   = Tháng 1
 *   [2,3]   = Tháng 2
 *   [4,5]   = Tháng 3
 *   [6,7]   = Tháng 4
 *   [8,9]   = Tháng 5
 *   [10,11] = Tháng 6
 *   [12,13] = Tháng 7
 *   [14,15] = Tháng 8
 *   [16,17] = Tháng 9
 *   [18,19] = Tháng 10
 *   [20,21] = Tháng 11
 *   [22,23] = Tháng 12
 *
 * Ngày thực tế của mỗi tiết khí có thể sớm hoặc muộn hơn 1 ngày so với
 * giá trị cơ sở trong bảng này. Thông tin điều chỉnh được lưu trong
 * bảng YearMonthBit[].
 */
static const uint8_t days[24] = {
    6, 20, 4, 19, 6, 21,  // January  – March  base dates
    5, 20, 6, 21, 6, 21,  // April    – June   base dates
    7, 23, 8, 23, 8, 23,  // July     – September base dates
    8, 24, 8, 22, 7, 22,  // October  – December base dates
};


/*
 * Tên của 24 tiết khí theo thứ tự trong năm (bắt đầu từ tháng 1):
 * {"Tiểu Hàn", "Đại Hàn",
 "Lập Xuân", "Vũ Thủy",
 "Kinh Trập","Xuân Phân",
 "Thanh Minh", "Cốc Vũ",
 "Lập Hạ","Tiểu Mãn",
 "Mang Chủng", "Hạ Chí",
 "Tiểu Thử", "Đại Thử",
 "Lập Thu", "Xử Thử",
 "Bạch Lộ", "Thu Phân",
 "Hàn Lộ","Sương Giáng",
 "Lập Đông", "Tiểu Tuyết",
 "Đại Tuyết","Đông Chí"
}
 *
 * Thứ tự truyền thống để tham khảo:
 * Lập Xuân, Vũ Thủy, Kinh Trập, Xuân Phân, Thanh Minh, Cốc Vũ,
 * Lập Hạ, Tiểu Mãn, Mang Chủng, Hạ Chí, Tiểu Thử, Đại Thử,
 * Lập Thu, Xử Thử, Bạch Lộ, Thu Phân, Hàn Lộ, Sương Giáng,
 * Lập Đông, Tiểu Tuyết, Đại Tuyết, Đông Chí, Tiểu Hàn, Đại Hàn.
 */
const char JieQiStr[24][7] = {
    "小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏", "小满", "芒种", "夏至",
    "小暑", "大暑", "立秋", "处暑", "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪", "冬至",
};

/* Số ngày của từng tháng trong năm Dương lịch thường (không nhuận) */
const uint8_t MonthDayMax[12] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
};

/*********************************************************************************************************
 * @brief  Lấy ngày trong tháng của tiết khí gần nhất với ngày được chỉ định.
 *
 * Với một ngày Dương lịch cho trước, hàm xác định tiết khí tương ứng trong
 * tháng (nửa đầu tháng → tiết khí thứ nhất, nửa cuối tháng → tiết khí thứ hai)
 * và trả về ngày trong tháng của tiết khí đó thông qua tham số `JQdate`.
 *
 * Ngày cơ sở được tra cứu từ bảng `days[]`, sau đó được điều chỉnh ±1 ngày
 * dựa trên các cờ bit của từng năm trong bảng `YearMonthBit[]`.
 *
 * Phạm vi năm được hỗ trợ: 2000–2050.
 *
 * Ví dụ:
 *   GetJieQi(2007, 2, 8, &date) → date = 4
 *   (Tiết Lập Xuân năm 2007 rơi vào ngày 04/02.)
 *
 * @param[in]  myear    Năm Dương lịch (2000–2050).
 * @param[in]  mmonth   Tháng Dương lịch (1–12).
 * @param[in]  mday     Ngày Dương lịch, chỉ dùng để xác định nửa đầu hay
 *                      nửa cuối tháng (ngày < 15 → tiết khí thứ nhất,
 *                      ngược lại → tiết khí thứ hai).
 * @param[out] JQdate   Con trỏ nhận ngày trong tháng của tiết khí.
 * @return              1 nếu thành công, 0 nếu tham số đầu vào không hợp lệ.
 *
 * @author  Laipí
 * @date    2007-02-08
 ********************************************************************************************************/
uint8_t GetJieQi(uint16_t myear, uint8_t mmonth, uint8_t mday, uint8_t* JQdate) {
    uint8_t bak1, value, JQ;

    if ((myear < 2000) || (myear > 2050)) return 0;
    if ((mmonth == 0) || (mmonth > 12)) return 0;
    JQ = (mmonth - 1) * 2;  /* Chỉ số của tiết khí trong năm (0–23) */
    if (mday >= 15) JQ++;   /* Nửa cuối tháng → chọn tiết khí thứ hai */

    bak1 = YearMonthBit[(myear - 2000) * 3 + JQ / 8];  /* Byte chứa adjustment flag của tiết khí */
    value = ((bak1 << (JQ % 8)) & 0x80);               /* Trích xuất 1 adjustment bit */

    *JQdate = days[JQ];
    if (value != 0) {
         /* Dấu +- của phần điều chỉnh ±1 ngày phụ thuộc vào tiết khí và năm */
        if ((JQ == 1 || JQ == 11 || JQ == 18 || JQ == 21) && myear < 2044)
            (*JQdate)++;
        else
            (*JQdate)--;
    }
    return 1;
}

/*********************************************************************************************************
 * @brief  Lấy chỉ số của tiết khí kế tiếp và số ngày còn lại đến tiết khí đó.
 *
 * Với một ngày Dương lịch cho trước, hàm sẽ xác định:
 *   - Tiết khí sắp tới (hoặc đang diễn ra trong ngày hôm nay).
 *   - Số ngày còn lại đến tiết khí đó (0 nếu hôm nay đúng là ngày tiết khí).
 *
 * Kết quả được trả về dưới dạng chỉ số tiết khí (0–23, tương ứng với bảng
 * JieQiStr[]), đồng thời số ngày còn lại được ghi vào `*day`.
 *
 * Ví dụ:
 *   GetJieQiStr(2007, 2, 8, &day)
 *     → trả về chỉ số của tiết Vũ Thủy, day = 11
 *       (còn 11 ngày nữa đến tiết Vũ Thủy).
 *
 * @param[in]  myear   Năm Dương lịch (2000–2050).
 * @param[in]  mmonth  Tháng Dương lịch (1–12).
 * @param[in]  mday    Ngày Dương lịch (1–31).
 * @param[out] day     Con trỏ nhận số ngày còn lại đến tiết khí kế tiếp
 *                     (0 nếu hôm nay đúng là ngày tiết khí).
 * @return             Chỉ số tiết khí (0–23) nếu thành công, 0xFF nếu thất bại.
 *
 * @author  Laipí
 * @date    2007-02-08
 ********************************************************************************************************/
uint8_t GetJieQiStr(uint16_t myear, uint8_t mmonth, uint8_t mday, uint8_t* day) {
    uint8_t JQdate, JQ, MaxDay;

    if (GetJieQi(myear, mmonth, mday, &JQdate) == 0) return 0xFF;

    JQ = (mmonth - 1) * 2;  /* Chỉ số của tiết khí trong năm (0–23) */

    if (mday >= 15) JQ++;   /* Nửa cuối tháng → chọn tiết khí thứ hai */

    if (mday == JQdate)  /* Hôm nay đúng là ngày tiết khí */
    {
        *day = 0;
        return JQ;
    }
    /* Hôm nay không phải là ngày tiết khí — tính số ngày còn lại đến tiết khí kế tiếp */

    if (mday < JQdate)    /* Hôm nay trước ngày tiết khí của nửa tháng hiện tại */
    {
        mday = JQdate - mday;
    } else   /* Hôm nay sau ngày tiết khí của nửa tháng hiện tại */
    {
        JQ++;  /* Chuyển sang tiết khí kế tiếp */

        if (mday < 15) {
            /* Vẫn ở nửa đầu tháng: tiết khí kế tiếp là tiết khí thứ hai của tháng */
            GetJieQi(myear, mmonth, 15, &JQdate);
            mday = JQdate - mday;
        } else  /* Đã qua giữa tháng: tiết khí kế tiếp nằm ở tháng kế tiếp */
        {
            MaxDay = MonthDayMax[mmonth - 1];
            if (mmonth == 2)  /* Xử lý ngày nhuận của tháng Hai trong năm nhuận */
            {
                if ((myear % 4 == 0) && ((myear % 100 != 0) || (myear % 400 == 0))) MaxDay++;
            }
            if (++mmonth == 13) mmonth = 1;  /* Chuyển từ tháng 12 sang tháng 1 - Wrap around */
            GetJieQi(myear, mmonth, 1, &JQdate);
            mday = MaxDay - mday + JQdate;
        }
    }
    *day = mday;
    return JQ;
}

/* Số giây trong một năm: chỉ số 0 = năm thường (365 ngày), chỉ số 1 = năm nhuận (366 ngày) */
uint32_t SEC_PER_YR[2] = {31536000, 31622400};

/*
 * Số giây của từng tháng:
 *   [0][] = năm thường, [1][] = năm nhuận
 *   Th1=31, Th2=28/29, Th3=31, Th4=30, Th5=31, Th6=30,
 *   Th7=31, Th8=31, Th9=30, Th10=31, Th11=30, Th12=31
 */
uint32_t SEC_PER_MT[2][12] = {
    {2678400, 2419200, 2678400, 2592000, 2678400, 2592000, 2678400, 2678400, 2592000, 2678400, 2592000, 2678400},
    {2678400, 2505600, 2678400, 2592000, 2678400, 2592000, 2678400, 2678400, 2592000, 2678400, 2592000, 2678400},
};
#define SECOND_OF_DAY 86400   /* Number of seconds in one day */

/**
 * @brief  Kiểm tra một năm có phải là năm nhuận hay không.
 *
 * Áp dụng quy tắc năm nhuận của lịch Gregory:
 *   - Năm tròn thế kỷ (chia hết cho 100) chỉ là năm nhuận nếu đồng thời chia hết cho 400.
 *   - Các năm còn lại là năm nhuận nếu chia hết cho 4.
 *
 * @param  yr  Năm cần kiểm tra (ví dụ: 2024).
 * @return     1 nếu là năm nhuận, 0 nếu không.
 *
 * @author Denis
 */
int is_leap(int yr) {
    if (0 == (yr % 100))
        return (yr % 400 == 0) ? 1 : 0;
    else
        return (yr % 4 == 0) ? 1 : 0;
}

/**
 * @brief  Tính thứ trong tuần của một ngày Dương lịch.
 *
 * Sử dụng thuật toán của Tomohiko Sakamoto, cho kết quả chính xác
 * với mọi ngày trong lịch Gregory.
 *
 * @param  month  Tháng (1–12).
 * @param  day    Ngày trong tháng (1–31).
 * @param  year   Năm Dương lịch đầy đủ (ví dụ: 2024).
 * @return        Thứ trong tuần:
 *                0 = Chủ nhật, 1 = Thứ hai, … 6 = Thứ bảy.
 *
 * @author Denis
 */
unsigned char day_of_week_get(unsigned char month, unsigned char day, unsigned short year) {
    /* Month should be a number 0 to 11, Day should be a number 1 to 31 */
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    year -= (uint8_t)(month < 3);
    return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;
}

/**
 * @brief  Chuyển đổi Unix timestamp thành cấu trúc thời gian.
 *
 * Phân rã số giây kể từ mốc Unix (1970-01-01 00:00:00 UTC) thành các
 * trường thời gian và lưu vào `*result`:
 *   - tm_year : Năm Dương lịch (là năm tuyệt đối trong quá trình tính toán,
 *               được chuyển thành số năm kể từ YEAR0 trước khi trả về).
 *   - tm_mon  : Tháng (đánh số từ 0, tức 0 = tháng 1).
 *   - tm_mday : Ngày trong tháng (đánh số từ 1).
 *   - tm_hour : Giờ (0–23).
 *   - tm_min  : Phút (0–59).
 *   - tm_sec  : Giây (0–59).
 *   - tm_wday : Thứ trong tuần (0 = Chủ nhật … 6 = Thứ bảy).
 *
 * Năm nhuận được xử lý chính xác thông qua các hàm `is_leap()` và
 * bảng `SEC_PER_MT[][]`.
 * Khi kết thúc hàm, `tm_year` được chuyển thành số năm kể từ YEAR0
 * theo quy ước của POSIX `struct tm`.
 *
 * @param[in]  unix_time  Số giây kể từ mốc Unix
 *                        (1970-01-01 00:00:00 UTC).
 * @param[out] result     Con trỏ tới cấu trúc `devtm` nhận kết quả.
 */
void transformTime(uint32_t unix_time, struct devtm* result) {
    int leapyr = 0;
    uint32_t ltime = unix_time;

    memset(result, 0, sizeof(struct devtm));
    result->tm_year = EPOCH_YR;

     /* Trừ dần từng năm, có xét đến năm nhuận */
    while (1) {
        if (ltime < SEC_PER_YR[is_leap(result->tm_year)]) {
            break;
        }
        ltime -= SEC_PER_YR[is_leap(result->tm_year)];
        ++(result->tm_year);
    }

    leapyr = is_leap(result->tm_year);

    /* Trừ dần từng tháng trong năm hiện tại */
    while (1) {
        if (ltime < SEC_PER_MT[leapyr][result->tm_mon]) break;
        ltime -= SEC_PER_MT[leapyr][result->tm_mon];
        ++(result->tm_mon);
    }

    /* Tính ngày trong tháng, sau đó tính thời gian còn lại trong ngày */
    result->tm_mday = ltime / SEC_PER_DY;
    ++(result->tm_mday); /* Chuyển từ đánh số bắt đầu từ 0 sang bắt đầu từ 1 */
    ltime = ltime % SEC_PER_DY;

    result->tm_hour = ltime / SEC_PER_HR;
    ltime = ltime % SEC_PER_HR;

    result->tm_min = ltime / 60;
    result->tm_sec = ltime % 60;

    result->tm_wday = day_of_week_get(result->tm_mon + 1, result->tm_mday, result->tm_year);

    /* Chuyển năm tuyệt đối thành số năm kể từ YEAR0 */
    result->tm_year -= YEAR0;
}

/* Số ngày của từng tháng trong năm thường (không nhuận), từ tháng 1 đến tháng 12 */
uint8_t map[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/**
 * @brief  Return the number of days in a given month of a given year.
 *
 * Handles the February leap-year case using is_leap().
 * Month index 1 (February, where month % 12 == 1) gains an extra day in leap years.
 *
 * @param  year   Gregorian year (used to determine leap year status for February).
 * @param  month  Month index (0-based internally; caller should pass 1-based values
 *                since the function applies % 12 to handle wrap-around).
 * @return        Number of days in the specified month (28–31).
 */
uint8_t get_last_day(uint16_t year, uint8_t month) {
    if (month % 12 == 1) {
        return map[month % 12] + is_leap(year);
    }
    return map[month % 12];
}

/**
 * @brief  Trả về số ngày của một tháng trong một năm.
 *
 * Xử lý trường hợp tháng Hai của năm nhuận thông qua hàm `is_leap()`.
 * Tháng Hai (chỉ số tháng % 12 == 1) sẽ có thêm 1 ngày nếu là năm nhuận.
 *
 * @param  year   Năm Dương lịch (dùng để xác định năm nhuận đối với tháng Hai).
 * @param  month  Chỉ số tháng (đánh số từ 0; hàm sử dụng phép `% 12` để xử lý
 *                trường hợp vượt phạm vi).
 * @return        Số ngày của tháng tương ứng (28–31).
 */
uint8_t get_first_day_week(uint16_t year, uint8_t month) { return day_of_week_get(month, 1, year); }

/**
 * @brief  Chuyển đổi cấu trúc thời gian thành Unix timestamp.
 *
 * Đây là hàm ngược với `transformTime()`. Hàm tính tổng số giây kể từ
 * mốc Unix (1970-01-01 00:00:00 UTC) đến thời điểm được lưu trong
 * `*result`.
 *
 * Tổng số ngày đã trôi qua được tính theo các bước:
 *   1. Cộng 366 ngày cho mỗi năm nhuận kể từ năm 1970.
 *   2. Cộng 365 ngày cho mỗi năm thường kể từ năm 1970.
 *   3. Cộng số ngày đã trôi qua trong năm hiện tại
 *      (bao gồm các tháng đã qua và ngày trong tháng).
 *
 * Sau đó, tổng số ngày được chuyển thành số giây và cộng thêm
 * thời gian trong ngày (giờ, phút, giây).
 *
 * @param[in]  result  Con trỏ tới cấu trúc `devtm` chứa thời gian cần chuyển đổi.
 *                     Các trường được sử dụng: `tm_year` (năm Dương lịch),
 *                     `tm_mon`, `tm_mday`, `tm_hour`, `tm_min`, `tm_sec`.
 * @return             Unix timestamp (số giây kể từ
 *                     1970-01-01 00:00:00 UTC).
 */
uint32_t transformTimeStruct(struct devtm* result) {
    uint32_t Cyear = 0;
    /* Đếm số năm nhuận từ năm 1970 đến trước năm hiện tại */
    for (uint16_t i = 1970; i < result->tm_year; i++) {
        if (is_leap(i) == 1) Cyear++;
    }

    /* Tổng số ngày đã trôi qua:
       mỗi năm nhuận có 366 ngày, mỗi năm thường có 365 ngày */
    uint32_t CountDay =
        Cyear * (uint32_t)366 + (uint32_t)(result->tm_year - 1970 - Cyear) * (uint32_t)365 + result->tm_mday - 1;
    
    /* Cộng số ngày của các tháng đã trôi qua trong năm hiện tại */
    for (uint8_t i = 0; i < result->tm_mon - 1; i++) {
        CountDay += get_last_day(result->tm_year, i);
    }

    return (CountDay * SECOND_OF_DAY + (uint32_t)result->tm_sec + (uint32_t)result->tm_min * 60 +
            (uint32_t)result->tm_hour * 3600);
}

/**
 * @brief  Trả về số ngày tối đa của một tháng, có xét đến năm nhuận.
 *
 * Đây là phiên bản rút gọn của `get_last_day()`, sử dụng phép kiểm tra
 * năm nhuận đơn giản bằng cách xét năm có chia hết cho 4 hay không.
 *
 * Lưu ý: Hàm **không** xử lý ngoại lệ đối với các năm tròn thế kỷ
 * (chia hết cho 100 nhưng không chia hết cho 400). Nếu cần kết quả
 * chính xác theo lịch Gregory, hãy sử dụng `is_leap()`.
 *
 * @param  year   Năm (2 hoặc 4 chữ số). Năm được coi là năm nhuận nếu
 *                chia hết cho 4.
 * @param  month  Tháng Dương lịch (1–12).
 * @return        Số ngày của tháng (28–31).
 */
uint8_t thisMonthMaxDays(uint8_t year, uint8_t month) {
    if (year % 4 == 0 && month == 2)
        return MonthDayMax[month - 1] + 1; /* Tháng Hai năm nhuận có 29 ngày */
    else
        return MonthDayMax[month - 1];
}
