import os

def unit_test(tu, test_name):
    test_bin = tu.join_base("bin/testrunner")
    if not os.path.exists(test_bin):
        return tu.FAILURE("testrunner binary not available")
    run_res = tu.run(test_bin, [test_name])
    return tu.check(run_res, "unit test failed")

def render_test(tu, test_name, arg):
    ref_name, render_spec = arg
    local_timeout_secs = 5

    test_bin = tu.join_base("bin/testrunner")
    if not os.path.exists(test_bin):
        return tu.FAILURE("testrunner binary not available")

    render_spec = render_spec.format(base=tu.base_path)

    ref_file = tu.join_base("test/ref_images/{}.ppm".format(ref_name))
    temp_file = tu.join_base("build/test.ppm")

    if os.path.exists(temp_file):
        os.remove(temp_file)

    run_res = tu.run(test_bin, [render_spec + temp_file], timeout_secs=local_timeout_secs)

    check_res = tu.check(run_res, "failed to render scene")
    if not check_res:
        return check_res

    rc, outs, errs = tu.run(tu.join_base("test/img_cmp"), [temp_file, ref_file])
    return tu.check((rc, run_res[1], run_res[2]), "rendered image differed from reference")


def test_image_test(tu, test_name):
    ref_image = tu.join_base("test/ref_images/test_image_ref.ppm")
    temp_file = tu.join_base("build/test_image.ppm")

    test_bin = tu.join_base("bin/testrunner")
    if not os.path.exists(test_bin):
        return tu.FAILURE("testrunner binary not available")

    if os.path.exists(temp_file):
        os.remove(temp_file)

    run_res = tu.run(test_bin, ["testimage:" + temp_file])

    check_res = tu.check(run_res, "failed to generate test image")
    if not check_res:
        return check_res

    rc, outs, errs = tu.run("diff", [temp_file, ref_image])
    return tu.check((rc, run_res[1], run_res[2]), "generated image differed from reference")

def specialize(fun, arg):
    return lambda tu, tn, x=arg: fun(tu, tn, x)

all_tests = {
    'public.player.move_01': unit_test,
    'public.player.move_02': unit_test,
    'public.player.move_03': unit_test,

    'public.draw.test_image': test_image_test,

    'public.render.island01': specialize(render_test, ("public.island01", "frame:800:600:600:{base}/maps/island:147.436798:539.899414:88:0:")),
    'public.render.pyramids01': specialize(render_test, ("public.pyramids01", "frame:640:480:800:{base}/maps/pyramids:110.388893:887.813843:164:0:")),
    'public.render.valley01': specialize(render_test, ("public.valley01", "frame:640:480:800:{base}/maps/valley:997.808716:559.177063:230:0:")),
}


timeout_secs = 1

